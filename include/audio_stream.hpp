#pragma once

#include "apu.hpp"
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/Audio/SoundChannel.hpp>
#include <vector>
#include <cstdint>

/// SFML SoundStream wrapper that pulls samples from the APU ring buffer
class GBSoundStream : public sf::SoundStream {
public:
    explicit GBSoundStream(APU& apu) : m_apu(apu) {
        // Stereo, 44100 Hz
        initialize(2, APU::SAMPLE_RATE,
                   {sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight});
        m_buffer.resize(CHUNK_SIZE, 0);
    }

    ~GBSoundStream() override {
        stop();
    }

    // Delete copy
    GBSoundStream(const GBSoundStream&) = delete;
    GBSoundStream& operator=(const GBSoundStream&) = delete;

private:
    // ~12ms of audio per chunk at 44100 Hz stereo (1024 samples = 512 frames)
    static constexpr std::size_t CHUNK_SIZE = 1024;

    bool onGetData(Chunk& data) override {
        std::size_t got = m_apu.read_samples(m_buffer.data(), CHUNK_SIZE);

        // If we didn't get enough samples, pad with silence to avoid underrun
        if (got < CHUNK_SIZE) {
            for (std::size_t i = got; i < CHUNK_SIZE; i++)
                m_buffer[i] = 0;
        }

        data.samples = m_buffer.data();
        data.sampleCount = CHUNK_SIZE;
        return true; // Always continue streaming
    }

    void onSeek(sf::Time /*timeOffset*/) override {
        // No-op: this is a live stream, seeking is not meaningful
    }

    APU& m_apu;
    std::vector<std::int16_t> m_buffer;
};
