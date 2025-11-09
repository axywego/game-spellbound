#include "BuffsGenerator.hpp"

std::optional<std::unique_ptr<BuffItem>> BuffsGenerator::create(const Player& player) {

    if(generateByChance(30) == false){
        return std::nullopt;
    }
    auto allCountBuffs = static_cast<unsigned>(StatType::Count);

    if (player.getCurrentMana() == -1.f) {
        allCountBuffs -= 3;
    }

    const auto countBuffs = generate8Bytes(1, 2);

    std::string nameBuff;

    std::vector<StatModifier> buffs;
    buffs.reserve(countBuffs);
    for (auto i = 0; i < countBuffs; ++i) {
        switch (generate8Bytes(0, allCountBuffs - 1)) {
            case 0: {
                const auto maxHealth = player.getMaxHealth();
                const auto val = generate8Bytes(1, static_cast<uint64_t>(maxHealth));
                buffs.emplace_back(StatType::Health, ModifierType::Flat, static_cast<float>(val));
                nameBuff += std::format("health +{};", val);
                break;
            }
            case 1: {
                const auto generated = generate8Bytes(1, 2);
                const auto val = static_cast<float>(generated) / 2.f;
                buffs.emplace_back(StatType::MaxHealth, ModifierType::Flat, val);
                nameBuff += std::format("max health +{};", val);
                break;
            }
            case 2: {
                const auto val = static_cast<float>(generate8Bytes(1, 2)) / 2.f;
                buffs.emplace_back(StatType::Damage, ModifierType::Flat, val);
                nameBuff += std::format("damage +{};", val);
                break;
            }
            case 3: {
                const auto val = generate8Bytes(10, 30);
                buffs.emplace_back(StatType::Speed, ModifierType::Flat, val);
                nameBuff += std::format("speed +{};", val);
                break;
            }
            case 4: {
                const float val = generate8Bytes(1, static_cast<uint64_t>(player.getCurrentMana() + 1)) / 4.f;
                buffs.emplace_back(StatType::Mana, ModifierType::Flat, val);
                nameBuff += std::format("mana +{};", val);
                break;
            }
            case 5: {
                const auto generated = generate8Bytes(1, 2);
                const float val = static_cast<float>(generated) / 4.f;
                buffs.emplace_back(StatType::MaxMana, ModifierType::Flat, val);
                nameBuff += std::format("max mana +{};", val);
                break;
            }
            case 6: {
                constexpr auto val = -0.025f;
                buffs.emplace_back(StatType::ManaCost, ModifierType::Flat, val);
                nameBuff += std::format("mana cost {};", val);
                break;
            }
            default: break;
        }
    }

    return std::make_unique<BuffItem>(ResourceManager::getInstance().getTexture("buff_item"), nameBuff, buffs);
}