#pragma once

// this is from https://github.com/OpenMPT/openmpt
// see LICENSE.OPENMPT for details

namespace das {

enum I3DL2Preset {
    Generic = 0,
    PaddedCell,
    Room,
    Bathroom,
    LivingRoom,
    StoneRoom,
    Auditorium,
    ConcertHall,
    Cave,
    Arena,
    Hangar,
    CarpetedHallway,
    Hallway,
    StoneCorridor,
    Alley,
    Forest,
    City,
    Mountains,
    Quarry,
    Plain,
    ParkingLot,
    SewerPipe,
    Underwater
};

struct I3DL2ReverbProperties {
	int32_t lRoom;               // [-10000, 0]      default: -10000 mB
	int32_t lRoomHF;             // [-10000, 0]      default: 0 mB
	float flDecayTime;         // [0.1, 20.0]      default: 1.0 s
	float flDecayHFRatio;      // [0.1, 2.0]       default: 0.5
	int32_t lReflections;        // [-10000, 1000]   default: -10000 mB
	float flReflectionsDelay;  // [0.0, 0.3]       default: 0.02 s
	int32_t lReverb;             // [-10000, 2000]   default: -10000 mB
	float flReverbDelay;       // [0.0, 0.1]       default: 0.04 s
	float flDiffusion;         // [0.0, 100.0]     default: 100.0 %
	float flDensity;           // [0.0, 100.0]     default: 100.0 %
};

class I3DL2Reverb final {
protected:
    enum Parameters {
        kI3DL2ReverbRoom = 0,
        kI3DL2ReverbRoomHF,
        kI3DL2ReverbRoomRolloffFactor,	// Doesn't actually do anything :)
        kI3DL2ReverbDecayTime,
        kI3DL2ReverbDecayHFRatio,
        kI3DL2ReverbReflections,
        kI3DL2ReverbReflectionsDelay,
        kI3DL2ReverbReverb,
        kI3DL2ReverbReverbDelay,
        kI3DL2ReverbDiffusion,
        kI3DL2ReverbDensity,
        kI3DL2ReverbHFReference,
        kI3DL2ReverbQuality,
        kI3DL2ReverbNumParameters
    };

    enum QualityFlags {
        kMoreDelayLines = 0x01,
        kFullSampleRate = 0x02,
    };

    class DelayLine : private vector<float> {
        int32_t m_length;
        int32_t m_position;
        int32_t m_delayPosition;
    public:
        void Init(int32_t ms, int32_t padding, uint32_t sampleRate, int32_t delayTap = 0);
        void SetDelayTap(int32_t delayTap);
        void Advance();
        void Set(float value);
        float Get(int32_t offset) const;
        float Get() const;
    };

    float m_param[kI3DL2ReverbNumParameters];
    int32_t m_program = 0;

    // Calculated parameters
    uint32_t m_quality;
    float m_effectiveSampleRate = 0.0f;
    float m_diffusion;
    float m_roomFilter;
    float m_ERLevel;
    float m_ReverbLevelL;
    float m_ReverbLevelR;

    int32_t m_delayTaps[15];	// 6*L + 6*R + LR + Early L + Early R
    int32_t m_earlyTaps[2][6];
    float m_delayCoeffs[13][2];

    // State
    DelayLine m_delayLines[19];
    float m_filterHist[19];

    // Remaining frame for downsampled reverb
    float m_prevL;
    float m_prevR;
    bool m_remain = false;

    bool m_recalcParams = true;

    float m_sampleRate = 44100.0f;

public:
    I3DL2Reverb();

    void SetSampleRate ( float rate ) { m_recalcParams = true; m_sampleRate = rate; }
    void Process(float *pOutL, float *pOutR, uint32_t numFrames);
    float GetParameter(int32_t index);
    void SetParameter(int32_t index, float value);
    void PositionChanged();
    void SetReverbProperties(const I3DL2ReverbProperties & preset);

protected:
    float Room() const { return -10000.0f + m_param[kI3DL2ReverbRoom] * 10000.0f; }
    float RoomHF() const { return -10000.0f + m_param[kI3DL2ReverbRoomHF] * 10000.0f; }
    float RoomRolloffFactor() const { return m_param[kI3DL2ReverbRoomRolloffFactor] * 10.0f; }
    float DecayTime() const { return 0.1f + m_param[kI3DL2ReverbDecayTime] * 19.9f; }
    float DecayHFRatio() const { return 0.1f + m_param[kI3DL2ReverbDecayHFRatio] * 1.9f; }
    float Reflections() const { return -10000.0f + m_param[kI3DL2ReverbReflections] * 11000.0f; }
    float ReflectionsDelay() const { return m_param[kI3DL2ReverbReflectionsDelay] * 0.3f; }
    float Reverb() const { return -10000.0f + m_param[kI3DL2ReverbReverb] * 12000.0f; }
    float ReverbDelay() const { return m_param[kI3DL2ReverbReverbDelay] * 0.1f; }
    float Diffusion() const { return m_param[kI3DL2ReverbDiffusion] * 100.0f; }
    float Density() const { return m_param[kI3DL2ReverbDensity] * 100.0f; }
    float HFReference() const { return 20.0f + m_param[kI3DL2ReverbHFReference] * 19980.0f; }
    uint32_t Quality() const { return (uint32_t) round(m_param[kI3DL2ReverbQuality] * 3.0f); }

    void RecalculateI3DL2ReverbParams();

    void SetDelayTaps();
    void SetDecayCoeffs();
    float CalcDecayCoeffs(int32_t index);
};

}

#ifdef I3DL32_REVERB_IMPLEMENTATION

#include <cmath>

namespace das {

void I3DL2Reverb::DelayLine::Init(int32_t ms, int32_t padding, uint32_t sampleRate, int32_t delayTap) {
	m_length = (sampleRate * ms / 1000) + padding;
	m_position = 0;
	SetDelayTap(delayTap);
	assign(m_length, 0.0f);
}

void I3DL2Reverb::DelayLine::SetDelayTap(int32_t delayTap) {
	if(m_length > 0)
		m_delayPosition = (delayTap + m_position + m_length) % m_length;
}

void I3DL2Reverb::DelayLine::Advance() {
	if(--m_position < 0)
		m_position += m_length;
	if(--m_delayPosition < 0)
		m_delayPosition += m_length;
}

__forceinline void I3DL2Reverb::DelayLine::Set(float value) {
	at(m_position) = value;
}

float I3DL2Reverb::DelayLine::Get(int32_t offset) const {
	offset = (offset + m_position) % m_length;
	if(offset < 0)
		offset += m_length;
	return at(offset);
}

__forceinline float I3DL2Reverb::DelayLine::Get() const {
	return at(m_delayPosition);
}

I3DL2Reverb::I3DL2Reverb() {
	m_param[kI3DL2ReverbRoom] = 0.9f;
	m_param[kI3DL2ReverbRoomHF] = 0.99f;
	m_param[kI3DL2ReverbRoomRolloffFactor] = 0.0f;
	m_param[kI3DL2ReverbDecayTime] = 0.07f;
	m_param[kI3DL2ReverbDecayHFRatio] = 0.3842105f;
	m_param[kI3DL2ReverbReflections] = 0.672545433f;
	m_param[kI3DL2ReverbReflectionsDelay] = 0.233333333f;
	m_param[kI3DL2ReverbReverb] = 0.85f;
	m_param[kI3DL2ReverbReverbDelay] = 0.11f;
	m_param[kI3DL2ReverbDiffusion] = 1.0f;
	m_param[kI3DL2ReverbDensity] = 1.0f;
	m_param[kI3DL2ReverbHFReference] = (5000.0f - 20.0f) / 19980.0f;
	m_param[kI3DL2ReverbQuality] = 2.0f / 3.0f;
}

void I3DL2Reverb::Process(float * inp, float * outp, uint32_t numFrames) {
	if(m_recalcParams) {
		auto sampleRate = m_effectiveSampleRate;
		RecalculateI3DL2ReverbParams();
		// Resize and clear delay lines if quality has changed
		if(sampleRate != m_effectiveSampleRate)
			PositionChanged();
	}

    uint32_t frames = numFrames;
	if(!(m_quality & kFullSampleRate) && m_remain && frames > 0) {
		// Remaining frame from previous render call
		frames--;
		*outp++ = m_prevL;
		*outp++ = m_prevR;
		inp+=2;
		m_remain = false;
	}

	while(frames > 0) {
		// Apply room filter and insert into early reflection delay lines
		const float inL = *inp++;
		const float inRoomL = (m_filterHist[12] - inL) * m_roomFilter + inL;
		m_filterHist[12] = inRoomL;
		m_delayLines[15].Set(inRoomL);

		const float inR = *inp++;
		const float inRoomR = (m_filterHist[13] - inR) * m_roomFilter + inR;
		m_filterHist[13] = inRoomR;
		m_delayLines[16].Set(inRoomR);

		// Early reflections (left)
		float earlyL = m_delayLines[15].Get(m_earlyTaps[0][1]) * 0.68f
			- m_delayLines[15].Get(m_earlyTaps[0][2]) * 0.5f
			- m_delayLines[15].Get(m_earlyTaps[0][3]) * 0.62f
			- m_delayLines[15].Get(m_earlyTaps[0][4]) * 0.5f
			- m_delayLines[15].Get(m_earlyTaps[0][5]) * 0.62f;
		if(m_quality & kMoreDelayLines) {
			float earlyL2 = earlyL;
			earlyL = m_delayLines[13].Get() + earlyL * 0.618034f;
			m_delayLines[13].Set(earlyL2 - earlyL * 0.618034f);
		}
		const float earlyRefOutL = earlyL * m_ERLevel;
		m_filterHist[15] = m_delayLines[15].Get(m_earlyTaps[0][0]) + m_filterHist[15];
		m_filterHist[16] = m_delayLines[16].Get(m_earlyTaps[1][0]) + m_filterHist[16];

		// Lots of slightly different copy-pasta ahead
		float reverbL1, reverbL2, reverbL3, reverbR1, reverbR2, reverbR3;

		reverbL1 = -m_filterHist[15] * 0.707f;
		reverbL2 = m_filterHist[16] * 0.707f + reverbL1;
		reverbR2 = reverbL1 - m_filterHist[16] * 0.707f;

		m_filterHist[5] = (m_filterHist[5] - m_delayLines[5].Get()) * m_delayCoeffs[5][1] + m_delayLines[5].Get();
		reverbL1 = m_filterHist[5] * m_delayCoeffs[5][0] + reverbL2 * m_diffusion;
		m_delayLines[5].Set(reverbL2 - reverbL1 * m_diffusion);
		reverbL2 = reverbL1;
		reverbL3 = -0.15f * reverbL1;

		m_filterHist[4] = (m_filterHist[4] - m_delayLines[4].Get()) * m_delayCoeffs[4][1] + m_delayLines[4].Get();
		reverbL1 = m_filterHist[4] * m_delayCoeffs[4][0] + reverbL2 * m_diffusion;
		m_delayLines[4].Set(reverbL2 - reverbL1 * m_diffusion);
		reverbL2 = reverbL1;
		reverbL3 -= reverbL1 * 0.2f;

		if(m_quality & kMoreDelayLines) {
			m_filterHist[3] = (m_filterHist[3] - m_delayLines[3].Get()) * m_delayCoeffs[3][1] + m_delayLines[3].Get();
			reverbL1 = m_filterHist[3] * m_delayCoeffs[3][0] + reverbL2 * m_diffusion;
			m_delayLines[3].Set(reverbL2 - reverbL1 * m_diffusion);
			reverbL2 = reverbL1;
			reverbL3 += 0.35f * reverbL1;

			m_filterHist[2] = (m_filterHist[2] - m_delayLines[2].Get()) * m_delayCoeffs[2][1] + m_delayLines[2].Get();
			reverbL1 = m_filterHist[2] * m_delayCoeffs[2][0] + reverbL2 * m_diffusion;
			m_delayLines[2].Set(reverbL2 - reverbL1 * m_diffusion);
			reverbL2 = reverbL1;
			reverbL3 -= reverbL1 * 0.38f;
		}
		m_delayLines[17].Set(reverbL2);

		reverbL1 = m_delayLines[17].Get() * m_delayCoeffs[12][0];
		m_filterHist[17] = (m_filterHist[17] - reverbL1) * m_delayCoeffs[12][1] + reverbL1;

		m_filterHist[1] = (m_filterHist[1] - m_delayLines[1].Get()) * m_delayCoeffs[1][1] + m_delayLines[1].Get();
		reverbL1 = m_filterHist[17] * m_diffusion + m_filterHist[1] * m_delayCoeffs[1][0];
		m_delayLines[1].Set(m_filterHist[17] - reverbL1 * m_diffusion);
		reverbL2 = reverbL1;
		float reverbL4 = reverbL1 * 0.38f;

		m_filterHist[0] = (m_filterHist[0] - m_delayLines[0].Get()) * m_delayCoeffs[0][1] + m_delayLines[0].Get();
		reverbL1 = m_filterHist[0] * m_delayCoeffs[0][0] + reverbL2 * m_diffusion;
		m_delayLines[0].Set(reverbL2 - reverbL1 * m_diffusion);
		reverbL3 -= reverbL1 * 0.38f;
		m_filterHist[15] = reverbL1;

		// Early reflections (right)
		float earlyR = m_delayLines[16].Get(m_earlyTaps[1][1]) * 0.707f
			- m_delayLines[16].Get(m_earlyTaps[1][2]) * 0.6f
			- m_delayLines[16].Get(m_earlyTaps[1][3]) * 0.5f
			- m_delayLines[16].Get(m_earlyTaps[1][4]) * 0.6f
			- m_delayLines[16].Get(m_earlyTaps[1][5]) * 0.5f;
		if(m_quality & kMoreDelayLines) {
			float earlyR2 = earlyR;
			earlyR = m_delayLines[14].Get() + earlyR * 0.618034f;
			m_delayLines[14].Set(earlyR2 - earlyR * 0.618034f);
		}
		const float earlyRefOutR = earlyR * m_ERLevel;

		m_filterHist[11] = (m_filterHist[11] - m_delayLines[11].Get()) * m_delayCoeffs[11][1] + m_delayLines[11].Get();
		reverbR1 = m_filterHist[11] * m_delayCoeffs[11][0] + reverbR2 * m_diffusion;
		m_delayLines[11].Set(reverbR2 - reverbR1 * m_diffusion);
		reverbR2 = reverbR1;

		m_filterHist[10] = (m_filterHist[10] - m_delayLines[10].Get()) * m_delayCoeffs[10][1] + m_delayLines[10].Get();
		reverbR1 = m_filterHist[10] * m_delayCoeffs[10][0] + reverbR2 * m_diffusion;
		m_delayLines[10].Set(reverbR2 - reverbR1 * m_diffusion);
		reverbR3 = reverbL4 - reverbR2 * 0.15f - reverbR1 * 0.2f;
		reverbR2 = reverbR1;

		if(m_quality & kMoreDelayLines) {
			m_filterHist[9] = (m_filterHist[9] - m_delayLines[9].Get()) * m_delayCoeffs[9][1] + m_delayLines[9].Get();
			reverbR1 = m_filterHist[9] * m_delayCoeffs[9][0] + reverbR2 * m_diffusion;
			m_delayLines[9].Set(reverbR2 - reverbR1 * m_diffusion);
			reverbR2 = reverbR1;
			reverbR3 += reverbR1 * 0.35f;

			m_filterHist[8] = (m_filterHist[8] - m_delayLines[8].Get()) * m_delayCoeffs[8][1] + m_delayLines[8].Get();
			reverbR1 = m_filterHist[8] * m_delayCoeffs[8][0] + reverbR2 * m_diffusion;
			m_delayLines[8].Set(reverbR2 - reverbR1 * m_diffusion);
			reverbR2 = reverbR1;
			reverbR3 -= reverbR1 * 0.38f;
		}
		m_delayLines[18].Set(reverbR2);

		reverbR1 = m_delayLines[18].Get() * m_delayCoeffs[12][0];
		m_filterHist[18] = (m_filterHist[18] - reverbR1) * m_delayCoeffs[12][1] + reverbR1;

		m_filterHist[7] = (m_filterHist[7] - m_delayLines[7].Get()) * m_delayCoeffs[7][1] + m_delayLines[7].Get();
		reverbR1 = m_filterHist[18] * m_diffusion + m_filterHist[7] * m_delayCoeffs[7][0];
		m_delayLines[7].Set(m_filterHist[18] - reverbR1 * m_diffusion);
		reverbR2 = reverbR1;

		float lateRevOutL = (reverbL3 + reverbR1 * 0.38f) * m_ReverbLevelL;

		m_filterHist[6] = (m_filterHist[6] - m_delayLines[6].Get()) * m_delayCoeffs[6][1] + m_delayLines[6].Get();
		reverbR1 = m_filterHist[6] * m_delayCoeffs[6][0] + reverbR2 * m_diffusion;
		m_delayLines[6].Set(reverbR2 - reverbR1 * m_diffusion);
		m_filterHist[16] = reverbR1;

		float lateRevOutR = (reverbR3 - reverbR1 * 0.38f) * m_ReverbLevelR;

		float outL = earlyRefOutL + lateRevOutL;
		float outR = earlyRefOutR + lateRevOutR;

		for(auto &line : m_delayLines)
			line.Advance();

		if(!(m_quality & kFullSampleRate)) {
			*outp++ = (outL + m_prevL) * 0.5f;
			*outp++ = (outR + m_prevR) * 0.5f;
			m_prevL = outL;
			m_prevR = outR;
			inp += 2;
			if(frames-- == 1)
			{
				m_remain = true;
				break;
			}
		}
		*outp++ = outL;
		*outp++ = outR;
		frames--;
	}
}

static I3DL2ReverbProperties ReverbPresets[] = {
	{ -1000, -100, 1.49f,0.83f, -2602,0.007f,   200,0.011f,100.0f,100.0f }, // Generic
	{ -1000,-6000, 0.17f,0.10f, -1204,0.001f,   207,0.002f,100.0f,100.0f }, // Padded Cell
	{ -1000, -454, 0.40f,0.83f, -1646,0.002f,    53,0.003f,100.0f,100.0f }, // Room
	{ -1000,-1200, 1.49f,0.54f,  -370,0.007f,  1030,0.011f,100.0f, 60.0f }, // Bathroom
	{ -1000,-6000, 0.50f,0.10f, -1376,0.003f, -1104,0.004f,100.0f,100.0f }, // Living Room
	{ -1000, -300, 2.31f,0.64f,  -711,0.012f,    83,0.017f,100.0f,100.0f }, // Stone Room
	{ -1000, -476, 4.32f,0.59f,  -789,0.020f,  -289,0.030f,100.0f,100.0f }, // Auditorium
	{ -1000, -500, 3.92f,0.70f, -1230,0.020f,    -2,0.029f,100.0f,100.0f }, // Concert Hall
	{ -1000,    0, 2.91f,1.30f,  -602,0.015f,  -302,0.022f,100.0f,100.0f }, // Cave
	{ -1000, -698, 7.24f,0.33f, -1166,0.020f,    16,0.030f,100.0f,100.0f }, // Arena
	{ -1000,-1000,10.05f,0.23f,  -602,0.020f,   198,0.030f,100.0f,100.0f }, // Hangar
	{ -1000,-4000, 0.30f,0.10f, -1831,0.002f, -1630,0.030f,100.0f,100.0f }, // Carpeted Hallway
	{ -1000, -300, 1.49f,0.59f, -1219,0.007f,   441,0.011f,100.0f,100.0f }, // Hallway
	{ -1000, -237, 2.70f,0.79f, -1214,0.013f,   395,0.020f,100.0f,100.0f }, // Stone Corridor
	{ -1000, -270, 1.49f,0.86f, -1204,0.007f,    -4,0.011f,100.0f,100.0f }, // Alley
	{ -1000,-3300, 1.49f,0.54f, -2560,0.162f,  -613,0.088f, 79.0f,100.0f }, // Forest
	{ -1000, -800, 1.49f,0.67f, -2273,0.007f, -2217,0.011f, 50.0f,100.0f }, // City
	{ -1000,-2500, 1.49f,0.21f, -2780,0.300f, -2014,0.100f, 27.0f,100.0f }, // Mountains
	{ -1000,-1000, 1.49f,0.83f,-10000,0.061f,   500,0.025f,100.0f,100.0f }, // Quarry
	{ -1000,-2000, 1.49f,0.50f, -2466,0.179f, -2514,0.100f, 21.0f,100.0f }, // Plain
	{ -1000,    0, 1.65f,1.50f, -1363,0.008f, -1153,0.012f,100.0f,100.0f }, // Parking Lot
	{ -1000,-1000, 2.81f,0.14f,   429,0.014f,   648,0.021f, 80.0f, 60.0f }, // Sewer Pipe
	{ -1000,-4000, 1.49f,0.10f,  -449,0.007f,  1700,0.011f,100.0f,100.0f }, // Underwater
};

void I3DL2Reverb::SetReverbProperties(const I3DL2ReverbProperties & preset) {
    m_param[kI3DL2ReverbRoom] = (preset.lRoom + 10000) / 10000.0f;
    m_param[kI3DL2ReverbRoomHF] = (preset.lRoomHF + 10000) / 10000.0f;
    m_param[kI3DL2ReverbRoomRolloffFactor] = 0.0f;
    m_param[kI3DL2ReverbDecayTime] = (preset.flDecayTime - 0.1f) / 19.9f;
    m_param[kI3DL2ReverbDecayHFRatio] = (preset.flDecayHFRatio - 0.1f) / 1.9f;
    m_param[kI3DL2ReverbReflections] = (preset.lReflections + 10000) / 11000.0f;
    m_param[kI3DL2ReverbReflectionsDelay] = preset.flReflectionsDelay / 0.3f;
    m_param[kI3DL2ReverbReverb] = (preset.lReverb + 10000) / 12000.0f;
    m_param[kI3DL2ReverbReverbDelay] = preset.flReverbDelay / 0.1f;
    m_param[kI3DL2ReverbDiffusion] = preset.flDiffusion / 100.0f;
    m_param[kI3DL2ReverbDensity] = preset.flDensity / 100.0f;
    m_param[kI3DL2ReverbHFReference] = (5000.0f - 20.0f) / 19980.0f;
    m_recalcParams = true;
}

float I3DL2Reverb::GetParameter(int32_t index) {
	if(index < kI3DL2ReverbNumParameters) {
		return m_param[index];
	}
	return 0;
}

template<class T, class C>
inline T Clamp(T val, const C lowerLimit, const C upperLimit) {
	if(val < lowerLimit) return lowerLimit;
	else if(val > upperLimit) return upperLimit;
	else return val;
}

void I3DL2Reverb::SetParameter(int32_t index, float value) {
	if(index < kI3DL2ReverbNumParameters) {
		value = Clamp(value, 0.0f, 1.0f);
		if(index == kI3DL2ReverbQuality)
			value = round(value * 3.0f) / 3.0f;
		m_param[index] = value;
		m_recalcParams = true;
	}
}

void I3DL2Reverb::PositionChanged() {
    for ( auto & fh : m_filterHist ) fh = 0;
	m_prevL = 0;
	m_prevR = 0;
	m_remain = false;

    uint32_t sampleRate = static_cast<uint32_t>(m_effectiveSampleRate);
    m_delayLines[0].Init(67, 5, sampleRate, m_delayTaps[0]);
    m_delayLines[1].Init(62, 5, sampleRate, m_delayTaps[1]);
    m_delayLines[2].Init(53, 5, sampleRate, m_delayTaps[2]);
    m_delayLines[3].Init(43, 5, sampleRate, m_delayTaps[3]);
    m_delayLines[4].Init(32, 5, sampleRate, m_delayTaps[4]);
    m_delayLines[5].Init(22, 5, sampleRate, m_delayTaps[5]);
    m_delayLines[6].Init(75, 5, sampleRate, m_delayTaps[6]);
    m_delayLines[7].Init(69, 5, sampleRate, m_delayTaps[7]);
    m_delayLines[8].Init(60, 5, sampleRate, m_delayTaps[8]);
    m_delayLines[9].Init(48, 5, sampleRate, m_delayTaps[9]);
    m_delayLines[10].Init(36, 5, sampleRate, m_delayTaps[10]);
    m_delayLines[11].Init(25, 5, sampleRate, m_delayTaps[11]);
    m_delayLines[12].Init(0, 0, 0);	// Dummy for array index consistency with both tap and coefficient arrays
    m_delayLines[13].Init(3, 0, sampleRate, m_delayTaps[13]);
    m_delayLines[14].Init(3, 0, sampleRate, m_delayTaps[14]);
    m_delayLines[15].Init(407, 1, sampleRate);
    m_delayLines[16].Init(400, 1, sampleRate);
    m_delayLines[17].Init(10, 0, sampleRate, -1);
    m_delayLines[18].Init(10, 0, sampleRate, -1);
}

void I3DL2Reverb::RecalculateI3DL2ReverbParams() {
	m_quality = Quality();
	m_effectiveSampleRate = static_cast<float>(m_sampleRate / ((m_quality & kFullSampleRate) ? 1u : 2u));
	// Diffusion
	m_diffusion = Diffusion() * (0.618034f / 100.0f);
	// Early Reflection Level
	m_ERLevel = min(pow(10.0f, (Room() + Reflections()) / (100.0f * 20.0f)), 1.0f) * 0.761f;
	// Room Filter
	float roomHF = pow(10.0f, RoomHF() / 100.0f / 10.0f);
	if(roomHF == 1.0f) {
		m_roomFilter = 0.0f;
	} else {
		float freq = (float) cos(HFReference() * (2.0f * M_PI) / m_effectiveSampleRate);
		float roomFilter = (freq * (roomHF + roomHF) - 2.0f + sqrt(freq * (roomHF * roomHF * freq * 4.0f) + roomHF * 8.0f - roomHF * roomHF * 4.0f - roomHF * freq * 8.0f)) / (roomHF + roomHF - 2.0f);
		m_roomFilter = Clamp(roomFilter, 0.0f, 1.0f);
	}
	SetDelayTaps();
	SetDecayCoeffs();
	m_recalcParams = false;
}

void I3DL2Reverb::SetDelayTaps() {
	// Early reflections
	static constexpr float delays[] = {
		1.0000f, 1.0000f, 0.0000f, 0.1078f, 0.1768f, 0.2727f,
		0.3953f, 0.5386f, 0.6899f, 0.8306f, 0.9400f, 0.9800f,
	};

	const float sampleRate = m_effectiveSampleRate;
	const float reflectionsDelay = ReflectionsDelay();
	const float reverbDelay = max(ReverbDelay(), 5.0f / 1000.0f);
	m_earlyTaps[0][0] = static_cast<int32_t>((reverbDelay + reflectionsDelay + 7.0f / 1000.0f) * sampleRate);
	for(uint32_t i = 1; i < 12; i++) {
		m_earlyTaps[i % 2u][i / 2u] = static_cast<int32_t>((reverbDelay * delays[i] + reflectionsDelay) * sampleRate);
	}

	// Late reflections
	float density = min((Density() / 100.0f + 0.1f) * 0.9091f, 1.0f);
	float delayL = density * 67.0f / 1000.0f * sampleRate;
	float delayR = density * 75.0f / 1000.0f * sampleRate;
	for(int i = 0, power = 0; i < 6; i++) {
		power += i;
		float factor = pow(0.93f, static_cast<float>(power));
		m_delayTaps[i + 0] = static_cast<int32_t>(delayL * factor);
		m_delayTaps[i + 6] = static_cast<int32_t>(delayR * factor);
	}
	m_delayTaps[12] = static_cast<int32_t>(10.0f / 1000.0f * sampleRate);
	// Early reflections (extra delay lines)
	m_delayTaps[13] = static_cast<int32_t>(3.25f / 1000.0f * sampleRate);
	m_delayTaps[14] = static_cast<int32_t>(3.53f / 1000.0f * sampleRate);

	for(size_t d = 0; d < size(m_delayTaps); d++)
		m_delayLines[d].SetDelayTap(m_delayTaps[d]);
}

void I3DL2Reverb::SetDecayCoeffs() {
	float levelLtmp = 1.0f, levelRtmp = 1.0f;
	float levelL = 0.0f, levelR = 0.0f;

	levelLtmp *= CalcDecayCoeffs(5);
	levelRtmp *= CalcDecayCoeffs(11);
	levelL += levelLtmp * 0.0225f;
	levelR += levelRtmp * 0.0225f;

	levelLtmp *= CalcDecayCoeffs(4);
	levelRtmp *= CalcDecayCoeffs(10);
	levelL += levelLtmp * 0.04f;
	levelR += levelRtmp * 0.04f;

	if(m_quality & kMoreDelayLines) {
		levelLtmp *= CalcDecayCoeffs(3);
		levelRtmp *= CalcDecayCoeffs(9);
		levelL += levelLtmp * 0.1225f;
		levelR += levelRtmp * 0.1225f;

		levelLtmp *= CalcDecayCoeffs(2);
		levelRtmp *= CalcDecayCoeffs(8);
		levelL += levelLtmp * 0.1444f;
		levelR += levelRtmp * 0.1444f;
	}
	CalcDecayCoeffs(12);
	levelLtmp *= m_delayCoeffs[12][0] * m_delayCoeffs[12][0];
	levelRtmp *= m_delayCoeffs[12][0] * m_delayCoeffs[12][0];

	levelLtmp *= CalcDecayCoeffs(1);
	levelRtmp *= CalcDecayCoeffs(7);
	levelL += levelRtmp * 0.1444f;
	levelR += levelLtmp * 0.1444f;

	levelLtmp *= CalcDecayCoeffs(0);
	levelRtmp *= CalcDecayCoeffs(6);
	levelL += levelLtmp * 0.1444f;
	levelR += levelRtmp * 0.1444f;

	// Final Reverb Level
	float level = min(pow(10.0f, (Room() + Reverb()) / (100.0f * 20.0f)), 1.0f);
	float monoInv = 1.0f - ((levelLtmp + levelRtmp) * 0.5f);
	m_ReverbLevelL = level * sqrt(monoInv / levelL);
	m_ReverbLevelR = level * sqrt(monoInv / levelR);
}


float I3DL2Reverb::CalcDecayCoeffs(int32_t index) {
	float hfRef = (float) (2.0f * M_PI) / m_effectiveSampleRate * HFReference();
	float decayHFRatio = DecayHFRatio();
	if(decayHFRatio > 1.0f)
		hfRef = M_PI;

	float c1 = pow(10.0f, ((static_cast<float>(m_delayTaps[index]) / m_effectiveSampleRate) * -60.0f / DecayTime()) / 20.0f);
	float c2 = 0.0f;

	float c21 = (pow(c1, 2.0f - 2.0f / decayHFRatio) - 1.0f) / (1.0f - cos(hfRef));
	if(c21 != 0 && isfinite(c21)) {
		float c22 = -2.0f * c21 - 2.0f;
		float c23sq = c22 * c22 - c21 * c21 * 4.0f;
		float c23 = c23sq > 0.0f ? sqrt(c23sq) : 0.0f;
		c2 = (c23 - c22) / (c21 + c21);
		if(abs(c2) > 1.0f)
			c2 = (-c22 - c23) / (c21 + c21);
        if(isnan(c2))
            c2 = 0.0f;
	}
	m_delayCoeffs[index][0] = c1;
	m_delayCoeffs[index][1] = c2;

	c1 *= c1;
	float diff2 = m_diffusion * m_diffusion;
	return diff2 + c1 / (1.0f - diff2 * c1) * (1.0f - diff2) * (1.0f - diff2);
}

}

#endif

