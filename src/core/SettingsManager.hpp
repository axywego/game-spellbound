#pragma once

class SettingsManager {
private:
	float musicValue{0.f};
    float soundValue{0.f};
	bool isVerticalSync{false};

	SettingsManager() = default;
	~SettingsManager() = default;
public:
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

	static SettingsManager& getInstance();

	void init();

	void setMusicValue(float newMusicValue);
	void setSoundValue(float newSoundValue);
	void setIsVerticalSync(bool newIsVerticalSync);

	float getMusicValue() const;

    float getSoundValue() const;

    bool getIsVerticalSync() const;
};

