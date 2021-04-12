// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_ 1

#include "soloud.h"
#include "soloud_wav.h"

namespace Aloe {

  class AudioPlayer {
  public:
    AudioPlayer();
    ~AudioPlayer();

    /** Play the clip*/
    void PlaySound();
    /** Stop the clip*/
    void StopSound();

    /** Load a clip*/
    void LoadClip(const char* clip_path);

  private:
    /** SoLoud source*/
    SoLoud::Soloud source;
    /** SoLoud clip*/
    SoLoud::Wav clip;
    /** SoLoud handle*/
    SoLoud::handle handle;
  };

}

#endif