#include <iomanip>
#include <iostream>
#include <random>

#include "elona.hpp"
#include "randomtitlegenerator.hpp"




// MODIFY HERE.

// [begin, end)
constexpr auto begin = 0;
constexpr auto end = 1 * 10000 * 10000;
constexpr auto seaching_type = 34;
constexpr auto power_threshold = 545;

constexpr auto has_ehekatl_feat = true;
constexpr auto hammer_enhancement = 0;
constexpr auto weapon_type = WeaponType::melee;
constexpr auto level = 1;





using namespace hsprnd;


gentleman::elona::RandomTitleGenerator title_generator;




void process_one_title(gentleman::random::Generator& gen, int weapon_seed)
{
    const auto weapon_title = title_generator.generate(weapon_seed - 40000);
    gen.randomize(weapon_seed);
    const auto blood = 4 + gen.rnd(12);

    int type{};
    int power{};
    for (int i = 0; i < 50; ++i)
    {
        const auto seed = weapon_seed + level * 10 + i;
        gen.randomize(seed);
        const auto e_level = gen.rnd(5);
        const auto e_type = randomenc(gen, e_level, weapon_type);
        const auto e_power = randomencp(gen, has_ehekatl_feat, hammer_enhancement);
        const auto e_type2 = encadd(gen, e_type);
        if (e_type2 != 0)
        {
            if (e_type2 == 34)
            {
                if (gen.rnd(3))
                {
                    continue;
                }
            }
            type = e_type2;
            power = e_power;
            break;
        }
    }

    std::cout << weapon_seed << "," << ((weapon_seed - 50500) / 17 + 1) << "," << weapon_title << "," << get_e_desc(type, power) << "," << power << "," << blood << std::endl;
}



bool match_enchantment(gentleman::random::Generator& gen, int weapon_seed, int type, int threshold)
{
    for (int i = 0; i < 50; ++i)
    {
        const auto seed = weapon_seed + level * 10 + i;
        gen.randomize(seed);
        const auto e_level = gen.rnd(5);
        const auto e_type = randomenc(gen, e_level, weapon_type);
        const auto e_power = randomencp(gen, has_ehekatl_feat, hammer_enhancement);
        const auto e_type2 = encadd(gen, e_type);
        if (e_type2 != 0)
        {
            if (e_type2 == 34)
            {
                if (gen.rnd(3))
                {
                    continue;
                }
            }
            return e_type2 == type && e_power >= threshold;
        }
    }

    return false;
}



int main()
{
    title_generator.initialize();

    init_enclist_table();

    std::cout << "Id,Page,Name,Enc,Power,Blood" << std::endl;

    const auto page_begin = begin / 17;
    const auto page_end = end / 17;

    gentleman::random::Generator gen;

    for (int page = page_begin; page < page_end; ++page)
    {
        for (int i = 1; i < 17; ++i)
        {
            const auto weapon_seed = 50500 + page * 17 + i;
            const auto match = match_enchantment(gen, weapon_seed, seaching_type, power_threshold);
            if (match)
            {
                process_one_title(gen, weapon_seed);
            }
        }
    }

    return 0;
}
