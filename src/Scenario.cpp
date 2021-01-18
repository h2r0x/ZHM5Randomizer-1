#include "Scenario.h"

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

namespace hitman_randomizer {

const Scenario Scenario::NONE = Scenario(0);
const Scenario Scenario::LOCATION_PEACOCK_CASUAL = Scenario(1);
const Scenario Scenario::LOCATION_PEACOCK_PROFESSIONAL = Scenario(2);
const Scenario Scenario::LOCATION_PEACOCK_MASTER = Scenario(3);
const Scenario Scenario::LOCATION_OCTOPUS_CASUAL = Scenario(4);
const Scenario Scenario::LOCATION_OCTOPUS_PROFESSIONAL = Scenario(5);
const Scenario Scenario::LOCATION_OCTOPUS_MASTER = Scenario(6);
const Scenario Scenario::LOCATION_SPIDER_CASUAL = Scenario(7);
const Scenario Scenario::LOCATION_SPIDER_PROFESSIONAL = Scenario(8);
const Scenario Scenario::LOCATION_SPIDER_MASTER = Scenario(9);
const Scenario Scenario::LOCATION_TIGER_CASUAL = Scenario(10);
const Scenario Scenario::LOCATION_TIGER_PROFESSIONAL = Scenario(11);
const Scenario Scenario::LOCATION_TIGER_MASTER = Scenario(12);
const Scenario Scenario::LOCATION_BULL_CASUAL = Scenario(13);
const Scenario Scenario::LOCATION_BULL_PROFESSIONAL = Scenario(14);
const Scenario Scenario::LOCATION_BULL_MASTER = Scenario(15);
const Scenario Scenario::LOCATION_SNOWCRANE_CASUAL = Scenario(16);
const Scenario Scenario::LOCATION_SNOWCRANE_PROFESSIONAL = Scenario(17);
const Scenario Scenario::LOCATION_SNOWCRANE_MASTER = Scenario(18);
const Scenario Scenario::LOCATION_SHEEP_CASUAL = Scenario(19);
const Scenario Scenario::LOCATION_SHEEP_PROFESSIONAL = Scenario(20);
const Scenario Scenario::LOCATION_SHEEP_MASTER = Scenario(21);
const Scenario Scenario::LOCATION_FLAMINGO_CASUAL = Scenario(22);
const Scenario Scenario::LOCATION_FLAMINGO_PROFESSIONAL = Scenario(23);
const Scenario Scenario::LOCATION_FLAMINGO_MASTER = Scenario(24);
const Scenario Scenario::LOCATION_HIPPO_CASUAL = Scenario(25);
const Scenario Scenario::LOCATION_HIPPO_PROFESSIONAL = Scenario(26);
const Scenario Scenario::LOCATION_HIPPO_MASTER = Scenario(27);
const Scenario Scenario::LOCATION_MONGOOSE_CASUAL = Scenario(28);
const Scenario Scenario::LOCATION_MONGOOSE_PROFESSIONAL = Scenario(29);
const Scenario Scenario::LOCATION_MONGOOSE_MASTER = Scenario(30);
const Scenario Scenario::LOCATION_SKUNK_CASUAL = Scenario(31);
const Scenario Scenario::LOCATION_SKUNK_PROFESSIONAL = Scenario(32);
const Scenario Scenario::LOCATION_SKUNK_MASTER = Scenario(33);
const Scenario Scenario::LOCATION_MAGPIE_CASUAL = Scenario(34);
const Scenario Scenario::LOCATION_MAGPIE_PROFESSIONAL = Scenario(35);
const Scenario Scenario::LOCATION_MAGPIE_MASTER = Scenario(36);
const Scenario Scenario::LOCATION_RACCOON_CASUAL = Scenario(37);
const Scenario Scenario::LOCATION_RACCOON_PROFESSIONAL = Scenario(38);
const Scenario Scenario::LOCATION_RACCOON_MASTER = Scenario(39);
const Scenario Scenario::LOCATION_STINGRAY_CASUAL = Scenario(40);
const Scenario Scenario::LOCATION_STINGRAY_PROFESSIONAL = Scenario(41);
const Scenario Scenario::LOCATION_STINGRAY_MASTER = Scenario(42);
const Scenario Scenario::LOCATION_POLARBEAR_MODULE_002 = Scenario(43);
const Scenario Scenario::LOCATION_POLARBEAR_MODULE_002_B = Scenario(44);
const Scenario Scenario::LOCATION_POLARBEAR_MODULE_005 = Scenario(45);
const Scenario Scenario::LOCATION_COPPERHEAD = Scenario(46);
const Scenario Scenario::LOCATION_MAMBA = Scenario(47);
const Scenario Scenario::LOCATION_EBOLA = Scenario(48);
const Scenario Scenario::LOCATION_PYTHON = Scenario(49);
const Scenario Scenario::LOCATION_ZIKA = Scenario(50);
const Scenario Scenario::LOCATION_RABIES = Scenario(51);
const Scenario Scenario::LOCATION_FLU = Scenario(52);
const Scenario Scenario::LOCATION_COTTONMOUTH_CASUAL = Scenario(53);
const Scenario Scenario::LOCATION_COTTONMOUTH_PROFESSIONAL = Scenario(54);
const Scenario Scenario::LOCATION_COTTONMOUTH_MASTER = Scenario(55);
const Scenario Scenario::LOCATION_ANACONDA_CASUAL = Scenario(56);
const Scenario Scenario::LOCATION_ANACONDA_PROFESSIONAL = Scenario(57);
const Scenario Scenario::LOCATION_ANACONDA_MASTER = Scenario(58);
const Scenario Scenario::LOCATION_KINGCOBRA_CASUAL = Scenario(59);
const Scenario Scenario::LOCATION_KINGCOBRA_PROFESSIONAL = Scenario(60);
const Scenario Scenario::LOCATION_KINGCOBRA_MASTER = Scenario(61);
const Scenario Scenario::LOCATION_GARTERSNAKE_CASUAL = Scenario(62);
const Scenario Scenario::LOCATION_GARTERSNAKE_PROFESSIONAL = Scenario(63);
const Scenario Scenario::LOCATION_GARTERSNAKE_MASTER = Scenario(64);
const Scenario Scenario::LOCATION_PARISNOEL = Scenario(65);
const Scenario Scenario::LOCATION_MAMUSHI = Scenario(66);
const Scenario Scenario::LOCATION_HALLOWEEN = Scenario(67);

std::vector<std::string> scenarioNames = {"NONE",
                                          "LOCATION_PEACOCK_CASUAL",
                                          "LOCATION_PEACOCK_PROFESSIONAL",
                                          "LOCATION_PEACOCK_MASTER",
                                          "LOCATION_OCTOPUS_CASUAL",
                                          "LOCATION_OCTOPUS_PROFESSIONAL",
                                          "LOCATION_OCTOPUS_MASTER",
                                          "LOCATION_SPIDER_CASUAL",
                                          "LOCATION_SPIDER_PROFESSIONAL",
                                          "LOCATION_SPIDER_MASTER",
                                          "LOCATION_TIGER_CASUAL",
                                          "LOCATION_TIGER_PROFESSIONAL",
                                          "LOCATION_TIGER_MASTER",
                                          "LOCATION_BULL_CASUAL",
                                          "LOCATION_BULL_PROFESSIONAL",
                                          "LOCATION_BULL_MASTER",
                                          "LOCATION_SNOWCRANE_CASUAL",
                                          "LOCATION_SNOWCRANE_PROFESSIONAL",
                                          "LOCATION_SNOWCRANE_MASTER",
                                          "LOCATION_SHEEP_CASUAL",
                                          "LOCATION_SHEEP_PROFESSIONAL",
                                          "LOCATION_SHEEP_MASTER",
                                          "LOCATION_FLAMINGO_CASUAL",
                                          "LOCATION_FLAMINGO_PROFESSIONAL",
                                          "LOCATION_FLAMINGO_MASTER",
                                          "LOCATION_HIPPO_CASUAL",
                                          "LOCATION_HIPPO_PROFESSIONAL",
                                          "LOCATION_HIPPO_MASTER",
                                          "LOCATION_MONGOOSE_CASUAL",
                                          "LOCATION_MONGOOSE_PROFESSIONAL",
                                          "LOCATION_MONGOOSE_MASTER",
                                          "LOCATION_SKUNK_CASUAL",
                                          "LOCATION_SKUNK_PROFESSIONAL",
                                          "LOCATION_SKUNK_MASTER",
                                          "LOCATION_MAGPIE_CASUAL",
                                          "LOCATION_MAGPIE_PROFESSIONAL",
                                          "LOCATION_MAGPIE_MASTER",
                                          "LOCATION_RACCOON_CASUAL",
                                          "LOCATION_RACCOON_PROFESSIONAL",
                                          "LOCATION_RACCOON_MASTER",
                                          "LOCATION_STINGRAY_CASUAL",
                                          "LOCATION_STINGRAY_PROFESSIONAL",
                                          "LOCATION_STINGRAY_MASTER",
                                          "LOCATION_POLARBEAR_MODULE_002",
                                          "LOCATION_POLARBEAR_MODULE_002_B",
                                          "LOCATION_POLARBEAR_MODULE_005",
                                          "LOCATION_COPPERHEAD",
                                          "LOCATION_MAMBA",
                                          "LOCATION_EBOLA",
                                          "LOCATION_PYTHON",
                                          "LOCATION_ZIKA",
                                          "LOCATION_RABIES",
                                          "LOCATION_FLU",
                                          "LOCATION_COTTONMOUTH_CASUAL",
                                          "LOCATION_COTTONMOUTH_PROFESSIONAL",
                                          "LOCATION_COTTONMOUTH_MASTER",
                                          "LOCATION_ANACONDA_CASUAL",
                                          "LOCATION_ANACONDA_PROFESSIONAL",
                                          "LOCATION_ANACONDA_MASTER",
                                          "LOCATION_KINGCOBRA_CASUAL",
                                          "LOCATION_KINGCOBRA_PROFESSIONAL",
                                          "LOCATION_KINGCOBRA_MASTER",
                                          "LOCATION_GARTERSNAKE_CASUAL",
                                          "LOCATION_GARTERSNAKE_PROFESSIONAL",
                                          "LOCATION_GARTERSNAKE_MASTER",
                                          "LOCATION_PARISNOEL",
                                          "LOCATION_MAMUSHI",
                                          "LOCATION_HALLOWEEN" };

std::unordered_map<size_t, Scenario> scenario_hash_map{
    {0xA188F4EA6EDFFB8D, Scenario::LOCATION_PEACOCK_CASUAL},
    {0xE7ECC9541F696DE6, Scenario::LOCATION_PEACOCK_PROFESSIONAL},
    {0x0E79238C18D0255A, Scenario::LOCATION_PEACOCK_MASTER},
    {0x968E7E2996B91876, Scenario::LOCATION_OCTOPUS_CASUAL},
    {0xC8DE0034776C6503, Scenario::LOCATION_OCTOPUS_PROFESSIONAL},
    {0xE6A0FE00EC7B0D2C, Scenario::LOCATION_OCTOPUS_MASTER},
    {0x90E43B4C6DF9AA30, Scenario::LOCATION_SPIDER_CASUAL},
    {0x86CF853E9A4E74E9, Scenario::LOCATION_SPIDER_PROFESSIONAL},
    {0x59F54CE458F946C8, Scenario::LOCATION_SPIDER_MASTER},
    {0x19F4FCB4588887CC, Scenario::LOCATION_TIGER_CASUAL},
    {0x4DF9074B6A697DCA, Scenario::LOCATION_TIGER_PROFESSIONAL},
    {0x51EB10653A2EC128, Scenario::LOCATION_TIGER_MASTER},
    {0xB75476348E5E4FA3, Scenario::LOCATION_BULL_CASUAL},
    {0x728213C5B3B72783, Scenario::LOCATION_BULL_PROFESSIONAL},
    {0x0187B502A86892AD, Scenario::LOCATION_BULL_MASTER},
    {0x7E7E798A9322E0F3, Scenario::LOCATION_SNOWCRANE_CASUAL},
    {0xC0C940C4CDF5500C, Scenario::LOCATION_SNOWCRANE_PROFESSIONAL},
    {0xA6899D72AE5864AB, Scenario::LOCATION_SNOWCRANE_MASTER},
    {0x689DE6BE1BB5C6FC, Scenario::LOCATION_SHEEP_CASUAL},
    {0x59EF9ED46E738B4A, Scenario::LOCATION_SHEEP_PROFESSIONAL},
    {0xF709163305184135, Scenario::LOCATION_SHEEP_MASTER},
    {0xD8336E20BCF33A42, Scenario::LOCATION_FLAMINGO_CASUAL},
    {0x37F3010FB35CAF78, Scenario::LOCATION_FLAMINGO_PROFESSIONAL},
    {0x2AF4FC18F2D011DF, Scenario::LOCATION_FLAMINGO_MASTER},
    {0xE46E9C23A4E4FE8D, Scenario::LOCATION_HIPPO_CASUAL},
    {0x67F835152501447A, Scenario::LOCATION_HIPPO_PROFESSIONAL},
    {0x07C6A61D8CB2211C, Scenario::LOCATION_HIPPO_MASTER},
    {0x37B6A301F78E1368, Scenario::LOCATION_MONGOOSE_CASUAL},
    {0xB6EE741B5D551772, Scenario::LOCATION_MONGOOSE_PROFESSIONAL},
    {0xC1F3B175722433E3, Scenario::LOCATION_MONGOOSE_MASTER},
    {0xA65CAE31919C2F98, Scenario::LOCATION_SKUNK_CASUAL},
    {0x846A1BD3092F0DB2, Scenario::LOCATION_SKUNK_PROFESSIONAL},
    {0xF7D847376F25162F, Scenario::LOCATION_SKUNK_MASTER},
    {0xC79E2AF2F8A4BC0F, Scenario::LOCATION_MAGPIE_CASUAL},
    {0x458495CA7EE1775A, Scenario::LOCATION_MAGPIE_PROFESSIONAL},
    {0x8C53FA80EA17A154, Scenario::LOCATION_MAGPIE_MASTER},
    {0xC56FB2696B239B7C, Scenario::LOCATION_RACCOON_CASUAL},
    {0x282F3BE3744658A0, Scenario::LOCATION_RACCOON_PROFESSIONAL},
    {0x59B9532BED64C387, Scenario::LOCATION_RACCOON_MASTER},
    {0xDCBB26152C9F4A59, Scenario::LOCATION_STINGRAY_CASUAL},
    {0x6ABC8B2D74789369, Scenario::LOCATION_STINGRAY_PROFESSIONAL},
    {0xE065A72B786E9AD0, Scenario::LOCATION_STINGRAY_MASTER},
    {0x593DBC1FAF7DD2E9, Scenario::LOCATION_POLARBEAR_MODULE_002},
    {0x54F68B5E26DEA94F, Scenario::LOCATION_POLARBEAR_MODULE_002_B},
    {0x75F82B3197DE560F, Scenario::LOCATION_POLARBEAR_MODULE_005},
    {0x5DE502627120844C, Scenario::LOCATION_COPPERHEAD},
    {0x879D0659498F0361, Scenario::LOCATION_MAMBA},
    {0x6DA262F67A627183, Scenario::LOCATION_EBOLA},
    {0x9E980BA9B872DE4C, Scenario::LOCATION_PYTHON},
    {0x9909D46A365426A5, Scenario::LOCATION_ZIKA},
    {0xE3E0AB8AAC751210, Scenario::LOCATION_RABIES},
    {0x9066FB0001110B4A, Scenario::LOCATION_FLU},
    {0x058DF134A74AA43B, Scenario::LOCATION_COTTONMOUTH_CASUAL},
    {0xE5D8F649B0F5ECFD, Scenario::LOCATION_COTTONMOUTH_PROFESSIONAL},
    {0x6BF6D067643F451C, Scenario::LOCATION_COTTONMOUTH_MASTER},
    {0xC14E5FD57B850A16, Scenario::LOCATION_ANACONDA_CASUAL},
    {0x23D7115D36AEA98F, Scenario::LOCATION_ANACONDA_PROFESSIONAL},
    {0x047A8BA76569D965, Scenario::LOCATION_ANACONDA_MASTER},
    {0x6C4ECC05EBCCCD53, Scenario::LOCATION_KINGCOBRA_CASUAL},
    {0x8F5FA695057C2BB9, Scenario::LOCATION_KINGCOBRA_PROFESSIONAL},
    {0xE21ACC4D55F59DC6, Scenario::LOCATION_KINGCOBRA_MASTER},
    {0x4DBA93CA3AD5E69B, Scenario::LOCATION_GARTERSNAKE_CASUAL},
    {0x890C90B3889ADD91, Scenario::LOCATION_GARTERSNAKE_PROFESSIONAL},
    {0x67A61D2EC82493A0, Scenario::LOCATION_GARTERSNAKE_MASTER},
    {0x6B562575ED9EDDF9, Scenario::LOCATION_PARISNOEL},
    {0x65F90E434F042244, Scenario::LOCATION_MAMUSHI},
    {0x7C31E76753B3D36D, Scenario::LOCATION_HALLOWEEN}};

Scenario::Scenario(int index) : index(index) {}

std::string Scenario::string() const { return scenarioNames[index]; }

bool Scenario::operator==(const Scenario& scenario) const {
  return index == scenario.index;
}

Scenario::operator int() const { return index; }

Scenario Scenario::from_string(std::string str) {
  auto it = std::find(scenarioNames.begin(), scenarioNames.end(), str);
  if (it == scenarioNames.end()) return Scenario::NONE;
  return static_cast<Scenario>(std::distance(scenarioNames.begin(), it));
}

Scenario Scenario::from_SceneInitParams(const SSceneInitParameters& params) {
  auto hash = std::hash<SSceneInitParameters>()(params);
  if (scenario_hash_map.count(hash)) return scenario_hash_map.at(hash);
  return Scenario::NONE;
}

}  // namespace hitman_randomizer