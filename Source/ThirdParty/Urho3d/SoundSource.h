//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "AudioDefs.h"
#include "Component.h"

namespace Urho3D
{

class Audio;
class Sound;
class SoundStream;

// Compressed audio decode buffer length in milliseconds
static const int STREAM_BUFFER_LENGTH = 100;

/// %Sound source component with stereo position.
class URHO3D_API SoundSource : public Component
{
    OBJECT(SoundSource);
    
public:
    /// Construct.
    SoundSource(Context* context);
    /// Destruct. Remove self from the audio subsystem
    virtual ~SoundSource();
    /// Register object factory.
    static void RegisterObject(Context* context);
    
    /// Play a sound.
    void Play(Sound* sound);
    /// Play a sound with specified frequency.
    void Play(Sound* sound, float frequency);
    /// Play a sound with specified frequency and gain.
    void Play(Sound* sound, float frequency, float gain);
    /// Play a sound with specified frequency, gain and panning.
    void Play(Sound* sound, float frequency, float gain, float panning);
    /// Start playing a sound stream.
    void Play(SoundStream* stream);
    /// Stop playback.
    void Stop();
    /// Set sound type, determines the master gain group.
    void SetSoundType(SoundType type);
    /// Set frequency.
    void SetFrequency(float frequency);
    /// Set gain. 0.0 is silence, 1.0 is full volume.
    void SetGain(float gain);
    /// Set attenuation. 1.0 is unaltered. Used for distance attenuated playback.
    void SetAttenuation(float attenuation);
    /// Set stereo panning. -1.0 is full left and 1.0 is full right.
    void SetPanning(float panning);
   /// Set whether sound source will be automatically removed from the scene node when playback stops.
    void SetAutoRemove(bool enable);
    /// Set new playback position.
    void SetPlayPosition(signed char* pos);
    
    /// Return sound.
    Sound* GetSound() const { return sound_; }
    /// Return playback position.
    volatile signed char* GetPlayPosition() const { return position_; }
    /// Return sound type, determines the master gain group.
    SoundType GetSoundType() const { return soundType_; }
    /// Return playback time position.
    float GetTimePosition() const { return timePosition_; }
    /// Return frequency.
    float GetFrequency() const { return frequency_; }
    /// Return gain.
    float GetGain() const { return gain_; }
    /// Return attenuation.
    float GetAttenuation() const { return attenuation_; }
    /// Return stereo panning.
    float GetPanning() const { return panning_; }
    /// Return autoremove mode.
    bool GetAutoRemove() const { return autoRemove_; }
    /// Return whether is playing.
    bool IsPlaying() const;
    
    /// Update the sound source. Perform subclass specific operations. Called by Audio.
    virtual void Update(float timeStep);
    /// Mix sound source output to a 32-bit clipping buffer. Called by Audio.
    void Mix(int* dest, unsigned samples, int mixRate, bool stereo, bool interpolation);
    
    /// Set sound attribute.
    void SetSoundAttr(ResourceRef value);
    /// Set sound position attribute.
    void SetPositionAttr(int value);
    /// Return sound attribute.
    ResourceRef GetSoundAttr() const;
    /// Set sound playing attribute
    void SetPlayingAttr(bool value);
    /// Return sound position attribute.
    int GetPositionAttr() const;
    
protected:
    /// Audio subsystem.
    WeakPtr<Audio> audio_;
    /// SoundSource type, determines the master gain group.
    SoundType soundType_;
    /// Frequency.
    float frequency_;
    /// Gain.
    float gain_;
    /// Attenuation.
    float attenuation_;
    /// Stereo panning.
    float panning_;
    /// Autoremove timer.
    float autoRemoveTimer_;
    /// Autoremove flag.
    bool autoRemove_;
    
private:
    /// Play a sound without locking the audio mutex. Called internally.
    void PlayLockless(Sound* sound);
    /// Play a sound stream without locking the audio mutex. Called internally.
    void PlayLockless(SharedPtr<SoundStream> stream);
    /// Stop sound without locking the audio mutex. Called internally.
    void StopLockless();
    /// Set new playback position without locking the audio mutex. Called internally.
    void SetPlayPositionLockless(signed char* position);
    /// Mix mono sample to mono buffer.
    void MixMonoToMono(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix mono sample to stereo buffer.
    void MixMonoToStereo(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix mono sample to mono buffer interpolated.
    void MixMonoToMonoIP(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix mono sample to stereo buffer interpolated.
    void MixMonoToStereoIP(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix stereo sample to mono buffer.
    void MixStereoToMono(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix stereo sample to stereo buffer.
    void MixStereoToStereo(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix stereo sample to mono buffer interpolated.
    void MixStereoToMonoIP(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Mix stereo sample to stereo buffer interpolated.
    void MixStereoToStereoIP(Sound* sound, int* dest, unsigned samples, int mixRate);
    /// Advance playback pointer without producing audible output.
    void MixZeroVolume(Sound* sound, unsigned samples, int mixRate);
    /// Advance playback pointer to simulate audio playback in headless mode.
    void MixNull(float timeStep);
    
    /// Sound that is being played.
    SharedPtr<Sound> sound_;
    /// Sound stream that is being played.
    SharedPtr<SoundStream> soundStream_;
    /// Playback position.
    volatile signed char *position_;
    /// Playback fractional position.
    volatile int fractPosition_;
    /// Playback time position.
    volatile float timePosition_;
    /// Decode buffer.
    SharedPtr<Sound> streamBuffer_;
    /// Position in stream buffer the next audio data from the stream will be written to.
    unsigned streamWritePosition_;
    /// Stream underrun flag.
    bool streamStopped_;
};

}
