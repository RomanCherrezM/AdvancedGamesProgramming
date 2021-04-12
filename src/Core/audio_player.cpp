// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/audio_player.h"

namespace Aloe {

  AudioPlayer::AudioPlayer() {
    handle = 0;
    source.init();
  }
  AudioPlayer::~AudioPlayer() {
    source.deinit();
  }

  void AudioPlayer::PlaySound() {
    handle = source.play(clip);
  }

  void AudioPlayer::StopSound() {
    source.stopAll();
  }

  void AudioPlayer::LoadClip(const char* clip_path) {
    clip.load(clip_path);
  }

}