#include "gorilla/ga.h"

#include "gorilla/devices/ga_xaudio2.h"

#define INITGUID
#include <windows.h>
#include <xaudio2.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

ga_DeviceImpl_XAudio2* gaX_device_open_xaudio2(gc_int32 in_numBuffers, gc_int32 in_numSamples, ga_Format* in_format)
{
  ga_DeviceImpl_XAudio2* ret = gcX_ops->allocFunc(sizeof(ga_DeviceImpl_XAudio2));
  HRESULT result;
  WAVEFORMATEX fmt;
  gc_int32 i;
  ret->devType = GA_DEVICE_TYPE_XAUDIO2;
  ret->numBuffers = in_numBuffers;
  ret->numSamples = in_numSamples;
  memcpy(&ret->format, in_format, sizeof(ga_Format));
  ret->sampleSize = ga_format_sampleSize(in_format);
  ret->nextBuffer = 0;
  ret->xa = 0;
  ret->master = 0;

  CoInitializeEx(NULL, COINIT_MULTITHREADED);
  result = XAudio2Create(&ret->xa, 0, XAUDIO2_DEFAULT_PROCESSOR);
  if(FAILED(result))
    goto cleanup;

  result = IXAudio2_CreateMasteringVoice(ret->xa, &ret->master, 2, 44100, 0, 0, 0);
  if(FAILED(result))
    goto cleanup;

  fmt.cbSize = sizeof(WAVEFORMATEX);
  ZeroMemory(&fmt, sizeof(WAVEFORMATEX));
  fmt.cbSize = sizeof(WAVEFORMATEX);
  fmt.wFormatTag = WAVE_FORMAT_PCM;
  fmt.nChannels = 2;
  fmt.wBitsPerSample = 16;
  fmt.nSamplesPerSec = 44100;
  fmt.nBlockAlign = fmt.nChannels * (fmt.wBitsPerSample / 8);
  fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;

  result = IXAudio2_CreateSourceVoice(ret->xa, &ret->source, &fmt, XAUDIO2_VOICE_NOPITCH, XAUDIO2_DEFAULT_FREQ_RATIO, 0, 0, 0);
  if(FAILED(result))
    goto cleanup;

  result =IXAudio2_StartEngine(ret->xa);
  if(FAILED(result))
    goto cleanup;

  result = IXAudio2SourceVoice_Start(ret->source, 0, XAUDIO2_COMMIT_NOW);
  if(FAILED(result))
    goto cleanup;

  ret->buffers = gcX_ops->allocFunc(ret->numBuffers * sizeof(void*));
  for(i = 0; i < ret->numBuffers; ++i)
    ret->buffers[i] = gcX_ops->allocFunc(ret->numSamples * ret->sampleSize);

  return ret;

cleanup:
  if(ret->source)
  {
    IXAudio2SourceVoice_Stop(ret->source, 0, XAUDIO2_COMMIT_NOW);
    IXAudio2SourceVoice_FlushSourceBuffers(ret->source);
    IXAudio2SourceVoice_DestroyVoice(ret->source);
  }
  if(ret->xa)
    IXAudio2_StopEngine(ret->xa);
  if(ret->master)
    IXAudio2MasteringVoice_DestroyVoice(ret->master);
  if(ret->xa)
    IXAudio2_Release(ret->xa);
  CoUninitialize();
  gcX_ops->freeFunc(ret);
  return 0;
}
gc_result gaX_device_close_xaudio2(ga_DeviceImpl_XAudio2* in_device)
{
  gc_int32 i;
  if(in_device->source)
  {
    IXAudio2SourceVoice_Stop(in_device->source, 0, XAUDIO2_COMMIT_NOW);
    IXAudio2SourceVoice_FlushSourceBuffers(in_device->source);
    IXAudio2SourceVoice_DestroyVoice(in_device->source);
  }
  if(in_device->xa)
    IXAudio2_StopEngine(in_device->xa);
  if(in_device->master)
    IXAudio2MasteringVoice_DestroyVoice(in_device->master);
  if(in_device->xa)
    IXAudio2_Release(in_device->xa);
  CoUninitialize();

  for(i = 0; i < in_device->numBuffers; ++i)
    gcX_ops->freeFunc(in_device->buffers[i]);
  gcX_ops->freeFunc(in_device->buffers);
  in_device->devType = GA_DEVICE_TYPE_UNKNOWN;
  in_device->source = 0;
  in_device->master = 0;
  in_device->xa = 0;
  gcX_ops->freeFunc(in_device);
  return GC_SUCCESS;
}
gc_int32 gaX_device_check_xaudio2(ga_DeviceImpl_XAudio2* in_device)
{
  gc_int32 ret = 0;
  XAUDIO2_VOICE_STATE state = { 0 };
  IXAudio2SourceVoice_GetState(in_device->source, &state);
  ret = in_device->numBuffers - state.BuffersQueued;
  return ret;
}
gc_result gaX_device_queue_xaudio2(ga_DeviceImpl_XAudio2* in_device,
                                   void* in_buffer)
{
  XAUDIO2_BUFFER buf;
  void* data;
  ZeroMemory(&buf, sizeof(XAUDIO2_BUFFER));
  buf.AudioBytes = in_device->numSamples * in_device->sampleSize;
  data = in_device->buffers[in_device->nextBuffer++];
  in_device->nextBuffer %= in_device->numBuffers;
  memcpy(data, in_buffer, buf.AudioBytes);
  buf.pAudioData = data;
  IXAudio2SourceVoice_SubmitSourceBuffer(in_device->source, &buf, 0);
  return GC_SUCCESS;
}
