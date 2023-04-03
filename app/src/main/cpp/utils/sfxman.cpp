//
// Created by aosp on 2023/2/26.
//

#include "sfxman.h"

#include <random>

#define SAMPLES_PER_SEC 8000
#define BUF_SAMPLES_MAX SAMPLES_PER_SEC * 5  // 5 seconds
#define DEFAULT_VOLUME 0.9f

static SfxMan *instance = new SfxMan();
static short sample_buf[BUF_SAMPLES_MAX];
static volatile bool bufferActive = false;

SfxMan *SfxMan::GetInstance() {
    return instance ? instance : (instance = new SfxMan());
}

static bool checkError(SLresult r, const char *what) {
    if (r != SL_RESULT_SUCCESS) {
        LOGW("SfxMan: Error %s (result %lu)", what, (long unsigned int)r);
        LOGW("DISABLING SOUND!");
        return true;
    }
    return false;
}

static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    bufferActive = false;
}

SfxMan::SfxMan() {
    // Note: this initialization code was mostly copied from the NDK audio sample.
    SLresult result;
    SLObjectItf engineObject = nullptr;
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject = nullptr;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = nullptr;
    SLObjectItf bqPlayerObject = nullptr;
    SLPlayItf bqPlayerPlay;
    SLEffectSendItf bqPlayerEffectSend;
    SLVolumeItf bqPlayerVolume;
    const SLEnvironmentalReverbSettings reverbSettings =
            SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    LOGD("SfxMan: initializing.");
    mPlayerBufferQueue = nullptr;

    // create engine
    result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    if (checkError(result, "creating engine")) return;

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (checkError(result, "realizing engine")) return;

    // get the engine interface, which is needed in order to create other objects
    result =
            (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (checkError(result, "getting engine interface")) return;

    // create output mix, with einitializingnvironmental reverb specified as a
    // non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)
            ->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    if (checkError(result, "creating output mix")) return;

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (checkError(result, "realizin goutput mix")) return;

    // get the environmental reverb interface
    // this could fail if the environmental reverb effect is not available,
    // either because the feature is not present, excessive CPU load, or
    // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
    result = (*outputMixObject)
            ->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                           &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)
                ->SetEnvironmentalReverbProperties(
                        outputMixEnvironmentalReverb, &reverbSettings);
    }
    // ignore unsuccessful result codes for environmental reverb, as it is
    // optional for this example

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,           1,
            SL_SAMPLINGRATE_8,           SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16, SL_SPEAKER_FRONT_CENTER,
            SL_BYTEORDER_LITTLEENDIAN};

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX,
                                          outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, nullptr};

    // create audio player
    const SLInterfaceID player_ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
            /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
    const SLboolean player_req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
            /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
    result = (*engineEngine)
            ->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc,
                                &audioSnk, 3, player_ids, player_req);
    if (checkError(result, "creating audio player")) return;

    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the play interface
    result = (*bqPlayerObject)
            ->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    if (checkError(result, "realizing audio player")) return;

    // get the buffer queue interface
    result = (*bqPlayerObject)
            ->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
                           &mPlayerBufferQueue);
    if (checkError(result, "getting buffer queue interface")) return;

    // register callback on the buffer queue
    result = (*mPlayerBufferQueue)
            ->RegisterCallback(mPlayerBufferQueue, bqPlayerCallback, nullptr);
    if (checkError(result, "registering callback on buffer queue")) return;

    // get the effect send interface
    result = (*bqPlayerObject)
            ->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND,
                           &bqPlayerEffectSend);
    if (checkError(result, "getting effect send interface")) return;

    // get the volume interface
    result = (*bqPlayerObject)
            ->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    if (checkError(result, "getting volume interface")) return;

    // set the player's state to playing
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (checkError(result, "setting play state to playing")) return;

    LOGD("SfxMan: initialization complete.");
    mInitOk = true;
}

bool SfxMan::IsIdle() { return !bufferActive; }

static const char *parseInt(const char *s, int *result) {
    *result = 0;
    while (*s >= '0' && *s <= '9') {
        *result = *result * 10 + (*s - '0');
        s++;
    }
    return s;
}

static int synth(int frequency, int duration, float amplitude,
                 short *sampleBuf, int samples) {
    int i;

    for (i = 0; i < samples; i++) {
        float t = i / (float)SAMPLES_PER_SEC;
        float v;
        if (frequency > 0) {
            v = amplitude * sin(frequency * t * 2 * M_PI) +
                (amplitude * 0.1f) * sin(frequency * 2 * t * 2 * M_PI);
        } else {
            int r = rand();
            r = r > 0 ? r : -r;
            v = amplitude * (-0.5f + (r % 1024) / 512.0f);
        }
        int value = (int)(v * 32768.0f);
        sampleBuf[i] = value < -32767 ? -32767 : value > 32767 ? 32767 : value;

        if (i > 0 && sampleBuf[i - 1] < 0 && sampleBuf[i] >= 0) {
            // start of new wave -- check if we have room for a full period of it
            int period_samples = (1.0f / frequency) * SAMPLES_PER_SEC;
            if (i + period_samples >= samples) break;
        }
    }

    return i;
}

static void taper(short *sampleBuf, int samples) {
    int i;
    const float TAPER_SAMPLES_FRACTION = 0.1f;
    int taper_samples = (int)(TAPER_SAMPLES_FRACTION * samples);
    for (i = 0; i < taper_samples && i < samples; i++) {
        float factor = i / (float)taper_samples;
        sampleBuf[i] = (short)((float)sampleBuf[i] * factor);
    }
    for (i = samples - taper_samples; i < samples; i++) {
        if (i < 0) continue;
        float factor = (samples - i) / (float)taper_samples;
        sampleBuf[i] = (short)((float)sampleBuf[i] * factor);
    }
}

void SfxMan::PlayTone(const char *tone) {
    if (!mInitOk) {
        LOGW("SfxMan: not playing sound because initialization failed.");
        return;
    }
    if (bufferActive) {
        // can't play -- the buffer is in use
        LOGW("SfxMan: can't play tone; buffer is active.");
        return;
    }

    // synth the tone
    int total_samples = 0;
    int num_samples;
    int frequency = 100;
    int duration = 50;
    int volume_int;
    float amplitude = DEFAULT_VOLUME;

    while (*tone) {
        switch (*tone) {
            case 'f':
                // set frequency
                tone = parseInt(tone + 1, &frequency);
                break;
            case 'd':
                // set duration
                tone = parseInt(tone + 1, &duration);
                break;
            case 'a':
                // set amplitude.
                tone = parseInt(tone + 1, &volume_int);
                amplitude = volume_int / 100.0f;
                amplitude = amplitude < 0.0f   ? 0.0f
                                               : amplitude > 1.0f ? 1.0f
                                                                  : amplitude;
                break;
            case '.':
                // synth
                num_samples = duration * SAMPLES_PER_SEC / 1000;
                if (num_samples > (BUF_SAMPLES_MAX - total_samples - 1)) {
                    num_samples = BUF_SAMPLES_MAX - total_samples - 1;
                }
                num_samples = synth(frequency, duration, amplitude,
                                     sample_buf + total_samples, num_samples);
                total_samples += num_samples;
                tone++;
                break;
            default:
                // ignore and advance to next character
                tone++;
        }
    }

    SLresult result;
    int total_size = total_samples * sizeof(short);
    if (total_size <= 0) {
        LOGW("Tone is empty. Not playing.");
        return;
    }

    taper(sample_buf, total_samples);

    bufferActive = true;
    result = (*mPlayerBufferQueue)
            ->Enqueue(mPlayerBufferQueue, sample_buf, total_size);
    if (result != SL_RESULT_SUCCESS) {
        LOGW("SfxMan: warning: failed to enqueue buffer: %lu",
             (unsigned long)result);
        return;
    }
}
