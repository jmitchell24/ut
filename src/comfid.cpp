//
// Created by james on 04/12/25.
//

#include "ut/algo/comfid.hpp"

#include "ut/string/view.hpp"
using namespace ut;

//
// std
//
#include <array>
using namespace std;

array<char const*, 128> static const g_nouns_singular =
{
    "whale",
    "jewelry",
    "head",
    "apple",
    "pencil",
    "shark",
    "apron",
    "nose",
    "desk",
    "sheep",
    "tie",
    "eye",
    "water",
    "melon",
    "table",
    "turkey",
    "bow",
    "lip",
    "lady",
    "finger",
    "laptop",
    "sparrow",
    "crest",
    "neck",
    "drum",
    "stick",
    "desktop",
    "tiger",
    "pocket",
    "watch",
    "hairs",
    "rasp",
    "berry",
    "mobile",
    "elephant",
    "belt",
    "tongue",
    "garlic",
    "freezer",
    "crow",
    "buckle",
    "chest",
    "ginger",
    "oven",
    "lion",
    "gown",
    "shoulder",
    "onion",
    "air",
    "bear",
    "jersey",
    "hand",
    "grain",
    "fan",
    "ostrich",
    "boxer",
    "finger",
    "gram",
    "pipe",
    "parrot",
    "sweater",
    "wrist",
    "gram",
    "spoon",
    "fox",
    "jacket",
    "waist",
    "rice",
    "pad",
    "wolf",
    "suitcase",
    "ankle",
    "mango",
    "iron",
    "box",
    "monkey",
    "ear",
    "ring",
    "leg",
    "milk",
    "music",
    "player",
    "panther",
    "sandal",
    "foot",
    "potato",
    "ear",
    "bud",
    "peacock",
    "shoe",
    "stomach",
    "balloon",
    "panda",
    "heart",
    "cauli",
    "flower",
    "tap",
    "plate",
    "frock",
    "brain",
    "butter",
    "lighter",
    "goat",
    "track",
    "suit",
    "toe",
    "cheese",
    "board",
    "buffalo",
    "shirt",
    "palm",
    "noodle",
    "koel",
    "bird",
    "suit",
    "rib",
    "pasta",
    "mouse",
    "tailor",
    "bird",
    "hat",
    "arm",
    "egg",
    "bulb",
    "frog",
    "cap",
    "back",
    "oil",
};

array static const g_nouns_plural =
{
    "whales",
    "jewelrys",
    "heads",
    "apples",
    "pencils",
    "sharks",
    "aprons",
    "noses",
    "desks",
    "sheeps",
    "ties",
    "eyes",
    "waters",
    "melons",
    "tables",
    "turkeys",
    "bows",
    "lips",
    "ladys",
    "fingers",
    "laptops",
    "sparrows",
    "crests",
    "necks",
    "drums",
    "sticks",
    "desktops",
    "tigers",
    "pockets",
    "watches",
    "hairses",
    "rasps",
    "berrys",
    "mobiles",
    "elephants",
    "belts",
    "tongues",
    "garlics",
    "freezers",
    "crows",
    "buckles",
    "chests",
    "gingers",
    "ovens",
    "lions",
    "gowns",
    "shoulders",
    "onions",
    "airs",
    "bears",
    "jerseys",
    "hands",
    "grains",
    "fans",
    "ostriches",
    "boxers",
    "fingers",
    "grams",
    "pipes",
    "parrots",
    "sweaters",
    "wrists",
    "grams",
    "spoons",
    "foxes",
    "jackets",
    "waists",
    "rices",
    "pads",
    "wolfs",
    "suitcases",
    "ankles",
    "mangos",
    "irons",
    "boxes",
    "monkeys",
    "ears",
    "rings",
    "legs",
    "milks",
    "musics",
    "players",
    "panthers",
    "sandals",
    "foots",
    "potatos",
    "ears",
    "buds",
    "peacocks",
    "shoes",
    "stomaches",
    "balloons",
    "pandas",
    "hearts",
    "caulis",
    "flowers",
    "taps",
    "plates",
    "frocks",
    "brains",
    "butters",
    "lighters",
    "goats",
    "tracks",
    "suits",
    "toes",
    "cheeses",
    "boards",
    "buffalos",
    "shirts",
    "palms",
    "noodles",
    "koels",
    "birds",
    "suits",
    "ribs",
    "pastas",
    "mouses",
    "tailors",
    "birds",
    "hats",
    "arms",
    "eggs",
    "bulbs",
    "frogs",
    "caps",
    "backs",
    "oils",
};


array static const g_past_tense_verbs =
{
	"amused",
 	"arose",
 	"babysat",
 	"was",
 	"beat",
 	"became",
 	"began",
 	"bent",
 	"bet",
 	"bound",
 	"bit",
 	"bled",
 	"blew",
	"blushed",
	"boiled",
 	"broke",
 	"brought",
 	"broadcast",
 	"built",
	"bought",
 	"caught",
	"checked",
 	"chose",
 	"came",
 	"cost",
	"connected",
 	"cut",
 	"dealt",
 	"dug",
 	"did",
 	"drew",
 	"drank",
 	"drove",
 	"ate",
	"faced",
	"filed",
 	"fell",
 	"fed",
 	"felt",
 	"fought",
 	"found",
 	"flew",
 	"forbade",
 	"forgot",
 	"forgave",
 	"froze",
 	"got",
 	"gave",
 	"went",
 	"grew",
 	"had",
	"hammered",
	"harassed",
 	"heard",
 	"hid",
 	"hit",
 	"held",
 	"hurt",
 	"kept",
 	"knew",
 	"laid",
 	"led",
 	"left",
 	"lent",
 	"let",
  	"lay",
 	"lit",
 	"lost",
 	"made",
 	"meant",
	"met",
 	"paid",
	"protected",
	"produced",
 	"put",
 	"quit",
 	"rode",
 	"rang",
 	"rose",
	"rolled",
	"rubbed",
 	"ran",
 	"said",
 	"saw",
 	"sold",
 	"sent",
 	"set",
 	"shook",
 	"shone",
 	"shot",
 	"showed",
 	"shut",
 	"sang",
 	"sank",
 	"sat",
 	"slept",
 	"slid",
 	"spoke",
 	"sped",
 	"spent",
 	"spun",
 	"spread",
	"served",
 	"stood",
 	"stole",
 	"stuck",
 	"stung",
 	"struck",
 	"swore",
 	"swept",
 	"swam",
 	"swung",
 	"took",
	"thanked",
	"trapped",
 	"taught",
 	"tore",
 	"told",
 	"thought",
 	"threw",
 	"understood",
 	"woke",
 	"wore",
 	"won",
 	"withdrew",
	"wandered",
 	"wrote",
	"yelled",
};

array<char const*, 128> static const g_adjectives =
{
	"unpleasant",
	"open",
	"appetizing",
	"annual",
	"murky",
	"ajar",
	"loyal",
	"spicy",
	"hollow",
	"primary",
	"loathsome",
	"weepy",
	"junior",
	"abounding",
	"sturdy",
	"harsh",
	"unsuitable",
	"elfin",
	"brisk",
	"corrupt",
	"French",
	"verifiable",
	"drab",
	"jagged",
	"profuse",
	"experienced",
	"grizzled",
	"clammy",
	"harebrained",
	"raspy",
	"horrific",
	"trustworthy",
	"colossal",
	"melancholy",
	"bashful",
	"growling",
	"sarcastic",
	"lonely",
	"shady",
	"barren",
	"illustrious",
	"powerful",
	"shameless",
	"motherly",
	"cooked",
	"cloistered",
	"awesome",
	"delicious",
	"secretive",
	"failing",
	"miserable",
	"frequent",
	"unsightly",
	"steep",
	"ugly",
	"trite",
	"dismal",
	"traumatic",
	"sour",
	"vast",
	"glib",
	"defenseless",
	"frayed",
	"worried",
	"ordinary",
	"Spanish",
	"candid",
	"tattered",
	"forked",
	"plausible",
	"livid",
	"well-off",
	"tranquil",
	"vengeful",
	"fussy",
	"cagey",
	"prickly",
	"brief",
	"optimal",
	"fatherly",
	"strange",
	"both",
	"vexed",
	"woebegone",
	"economic",
	"important",
	"tiny",
	"academic",
	"potable",
	"dual",
	"rare",
	"productive",
	"worrisome",
	"evasive",
	"efficient",
	"agitated",
	"admirable",
	"various",
	"obvious",
	"innate",
	"outlandish",
	"beloved",
	"subdued",
	"shabby",
	"acrid",
	"cool",
	"incompetent",
	"light",
	"frail",
	"sleepy",
	"tepid",
	"treasured",
	"private",
	"dysfunctional",
	"classic",
	"natural",
	"needless",
	"orange",
	"calm",
	"exclusive",
	"thunderous",
	"heartbreaking",
	"painstaking",
	"nutritious",
	"uppity",
	"empty",
	"smarmy",
	"rubbery",
};

array  static const g_objects =
{

	"album",
	"apple",
	"apron",
	"bag",
	"ball",
	"basket",
	"battery",
	"bench",
	"blanket",
	"lime",
	"blender",
	"bolster",
	"bookmark",
	"bottle",
	"box",
	"bracelet",
	"cable",
	"cage",
	"can",
	"candle",
	"cap",
	"carabiner",
	"carton",
	"cat",
	"charger",
	"cheese",
	"chessboard",
	"cleaner",
	"clip",
	"clipboard",
	"clock",
	"closet",
	"cloth",
	"clothespin",
	"coaster",
	"coat",
	"container",
	"cork",
	"cue",
	"cupcake",
	"desk",
	"diary",
	"dish",
	"drill",
	"dumbbell",
	"egg",
	"eraser",
	"football",
	"footrest",
	"globe",
	"glove",
	"glue gun",
	"goggles",
	"grinder",
	"hairbrush",
	"hammer",
	"hammock",
	"hanger",
	"headlamp",
	"hook",
	"hose",
	"jar",
	"jelly",
	"jump rope",
	"key",
	"lamp",
	"latch",
	"leash",
	"letter",
	"lint",
	"lipstick",
	"lunchbox",
	"marker",
	"matchbox",
	"microwave",
	"mirror",
	"mister",
	"mug",
	"napkin",
	"notebook",
	"notepad",
	"opener",
	"oven",
	"pan",
	"pen",
	"pencil",
	"photo",
	"picture",
	"pin",
	"pump",
	"puzzle",
	"rack",
	"raincoat",
	"rake",
	"rubber",
	"ruler",
	"scoop",
	"scraper",
	"screwdriver",
	"shampoo",
	"sharpener",
	"sheet",
	"shoelace",
	"soap",
	"spice",
	"sponge",
	"spray",
	"stopper",
	"strap",
	"straw",
	"strip",
	"sunglasses",
	"sunscreen",
	"tape",
	"tarp",
	"thermometer",
	"thermos",
	"toilet",
	"tongs",
	"toothpaste",
	"towel",
	"tray",
	"tweezers",
	"umbrella",
	"vacuum",
	"vase",
	"wheelbarrow",
	"whistle",
};

std::vector<std::string>  static const g_determiners_plural =
{
    "the", "my", "your", "his", "her", "our", "their", "no", "these", "those", "some", "many", "few", "several", "both", "us"
};

std::vector<std::string>  static const g_determiners_singular =
{
    "the", "my", "your", "his", "her", "our", "their", "no", "this", "that", "a", "an", "each", "every", "whichever", "said"
};

string pluralize(cstrparam noun)
{
    if (noun.endsWith('s') ||
        noun.endsWith('x') ||
        noun.endsWith('z') ||
        noun.endsWith("sh") ||
        noun.endsWith("ch") ||
        noun.endsWith("ss"))
        return noun.str() + "es";

    return noun.str() + "s";
}

// static bool startsWithVowelSound(string const& word)
// {
//     if (word.empty()) return false;
//
//     char first = std::tolower(word[0]);
//
//     // Special cases
//     if (word.substr(0, 3) == "uni" || word.substr(0, 2) == "eu" ||
//         word.substr(0, 3) == "one") {
//         return false;
//         }
//     if (word.substr(0, 3) == "hon" || word.substr(0, 4) == "hour") {
//         return true;
//     }
//
//     return (first == 'a' || first == 'e' || first == 'i' ||
//             first == 'o' || first == 'u');
// }

static string conjugateVerb(const std::string& base_verb)
{


    // Basic English conjugation rules
    size_t len = base_verb.length();

    if (base_verb[len-1] == 'y') {
        return base_verb.substr(0, len-1) + "ies";
    }

    if (base_verb[len-1] == 's' || base_verb[len-1] == 'x' ||
        base_verb[len-1] == 'z' ||
        (len >= 2 && base_verb.substr(len-2) == "ch") ||
        (len >= 2 && base_verb.substr(len-2) == "sh")) {
        return base_verb + "es";
        }

    return base_verb + "s";
}

static string selectDeterminer(bool is_plural, uint32_t det_bits)
{
    vector<string> valid_determiners;

    if (is_plural)
    {
        valid_determiners = g_determiners_plural;
    }
    else
    {
        valid_determiners = g_determiners_singular;
    }

    size_t det_idx = det_bits % valid_determiners.size();
    string determiner = valid_determiners[det_idx];

    return determiner;
}

static uint32_t scramble_simple(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
    return x;
}


static string intToSentence(uint32_t num)
{



	union Indices
	{
		struct
		{
			uint32_t is_plural: 1;
			uint32_t det_idx  : 4;
			uint32_t noun_idx : 7;
			uint32_t verb_idx : 7;
			uint32_t adj_idx  : 7;
			uint32_t obj_idx  : 6;
		};
		uint32_t value;
	};

	Indices idx;
	idx.value = scramble_simple(num);

    // Generate noun with plural suffix if needed

    string noun = idx.is_plural
        ? g_nouns_plural [idx.noun_idx % g_nouns_plural.size()]
        : g_nouns_singular [ idx.noun_idx % g_nouns_singular.size()];

	string determiner = idx.is_plural
		? g_determiners_plural [ idx.det_idx & g_determiners_plural.size() ]
		: g_determiners_singular [ idx.det_idx & g_determiners_singular.size() ];

    string subject = determiner + " " + noun;

    // Generate verb with conjugation
    string verb =
        g_past_tense_verbs[idx.verb_idx % g_past_tense_verbs.size()];

    // Get adjective and object
    string adjective = g_adjectives[idx.adj_idx % g_adjectives.size()];
    string object = g_objects[idx.obj_idx % g_objects.size()];

    return subject + " " + verb + " " + adjective + " " + object;
}

uint32_t sentenceToInt(const std::string& sentence) {
    // This would require parsing the sentence and reverse-mapping
    // Left as an exercise - would need word-to-index maps
    return 0;
}

string ComfID::toString() const
{
    return intToSentence(value);
}

#include <iostream>
using namespace std;
void ComfID::printDetBits()
{
    for (int i = 0; i < 10; ++i)
    {
        cout << i << " : " << intToSentence(i) << endl;
    }
}