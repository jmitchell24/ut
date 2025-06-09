//
// Generated from cli-spinners.json"
//

#pragma once
#include <vector>
#include <string>


#define UT_SPINNERS_COUNT 88
#define UT_EXPAND_SPINNERS(SPINNER) \
    SPINNER(dots) \
    SPINNER(dots2) \
    SPINNER(dots3) \
    SPINNER(dots4) \
    SPINNER(dots5) \
    SPINNER(dots6) \
    SPINNER(dots7) \
    SPINNER(dots8) \
    SPINNER(dots9) \
    SPINNER(dots10) \
    SPINNER(dots11) \
    SPINNER(dots12) \
    SPINNER(dots13) \
    SPINNER(dots14) \
    SPINNER(dots8Bit) \
    SPINNER(dotsCircle) \
    SPINNER(sand) \
    SPINNER(line) \
    SPINNER(line2) \
    SPINNER(pipe) \
    SPINNER(simpleDots) \
    SPINNER(simpleDotsScrolling) \
    SPINNER(star) \
    SPINNER(star2) \
    SPINNER(flip) \
    SPINNER(hamburger) \
    SPINNER(growVertical) \
    SPINNER(growHorizontal) \
    SPINNER(balloon) \
    SPINNER(balloon2) \
    SPINNER(noise) \
    SPINNER(bounce) \
    SPINNER(boxBounce) \
    SPINNER(boxBounce2) \
    SPINNER(triangle) \
    SPINNER(binary) \
    SPINNER(arc) \
    SPINNER(circle) \
    SPINNER(squareCorners) \
    SPINNER(circleQuarters) \
    SPINNER(circleHalves) \
    SPINNER(squish) \
    SPINNER(toggle) \
    SPINNER(toggle2) \
    SPINNER(toggle3) \
    SPINNER(toggle4) \
    SPINNER(toggle5) \
    SPINNER(toggle6) \
    SPINNER(toggle7) \
    SPINNER(toggle8) \
    SPINNER(toggle9) \
    SPINNER(toggle10) \
    SPINNER(toggle11) \
    SPINNER(toggle12) \
    SPINNER(toggle13) \
    SPINNER(arrow) \
    SPINNER(arrow2) \
    SPINNER(arrow3) \
    SPINNER(bouncingBar) \
    SPINNER(bouncingBall) \
    SPINNER(smiley) \
    SPINNER(monkey) \
    SPINNER(hearts) \
    SPINNER(clock) \
    SPINNER(earth) \
    SPINNER(material) \
    SPINNER(moon) \
    SPINNER(runner) \
    SPINNER(pong) \
    SPINNER(shark) \
    SPINNER(dqpb) \
    SPINNER(weather) \
    SPINNER(christmas) \
    SPINNER(grenade) \
    SPINNER(point) \
    SPINNER(layer) \
    SPINNER(betaWave) \
    SPINNER(fingerDance) \
    SPINNER(fistBump) \
    SPINNER(soccerHeader) \
    SPINNER(mindblown) \
    SPINNER(speaker) \
    SPINNER(orangePulse) \
    SPINNER(bluePulse) \
    SPINNER(orangeBluePulse) \
    SPINNER(timeTravel) \
    SPINNER(aesthetic) \
    SPINNER(dwarfFortress) \


namespace ut
{
    struct Spinner
    {
        int interval;
        std::vector<std::string> frames;
    };
}

namespace ut::spinners
{

    Spinner static const dots = { 80, // dots
        { "⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏", }
    };

    Spinner static const dots2 = { 80, // dots2
        { "⣾","⣽","⣻","⢿","⡿","⣟","⣯","⣷", }
    };

    Spinner static const dots3 = { 80, // dots3
        { "⠋","⠙","⠚","⠞","⠖","⠦","⠴","⠲","⠳","⠓", }
    };

    Spinner static const dots4 = { 80, // dots4
        { "⠄","⠆","⠇","⠋","⠙","⠸","⠰","⠠","⠰","⠸","⠙","⠋","⠇","⠆", }
    };

    Spinner static const dots5 = { 80, // dots5
        { "⠋","⠙","⠚","⠒","⠂","⠂","⠒","⠲","⠴","⠦","⠖","⠒","⠐","⠐","⠒","⠓","⠋", }
    };

    Spinner static const dots6 = { 80, // dots6
        { "⠁","⠉","⠙","⠚","⠒","⠂","⠂","⠒","⠲","⠴","⠤","⠄","⠄","⠤","⠴","⠲","⠒","⠂","⠂","⠒","⠚","⠙","⠉","⠁", }
    };

    Spinner static const dots7 = { 80, // dots7
        { "⠈","⠉","⠋","⠓","⠒","⠐","⠐","⠒","⠖","⠦","⠤","⠠","⠠","⠤","⠦","⠖","⠒","⠐","⠐","⠒","⠓","⠋","⠉","⠈", }
    };

    Spinner static const dots8 = { 80, // dots8
        { "⠁","⠁","⠉","⠙","⠚","⠒","⠂","⠂","⠒","⠲","⠴","⠤","⠄","⠄","⠤","⠠","⠠","⠤","⠦","⠖","⠒","⠐","⠐","⠒","⠓","⠋","⠉","⠈","⠈", }
    };

    Spinner static const dots9 = { 80, // dots9
        { "⢹","⢺","⢼","⣸","⣇","⡧","⡗","⡏", }
    };

    Spinner static const dots10 = { 80, // dots10
        { "⢄","⢂","⢁","⡁","⡈","⡐","⡠", }
    };

    Spinner static const dots11 = { 100, // dots11
        { "⠁","⠂","⠄","⡀","⢀","⠠","⠐","⠈", }
    };

    Spinner static const dots12 = { 80, // dots12
        { "⢀⠀","⡀⠀","⠄⠀","⢂⠀","⡂⠀","⠅⠀","⢃⠀","⡃⠀","⠍⠀","⢋⠀","⡋⠀","⠍⠁","⢋⠁","⡋⠁","⠍⠉","⠋⠉","⠋⠉","⠉⠙","⠉⠙","⠉⠩","⠈⢙","⠈⡙","⢈⠩","⡀⢙","⠄⡙","⢂⠩","⡂⢘","⠅⡘","⢃⠨","⡃⢐","⠍⡐","⢋⠠","⡋⢀","⠍⡁","⢋⠁","⡋⠁","⠍⠉","⠋⠉","⠋⠉","⠉⠙","⠉⠙","⠉⠩","⠈⢙","⠈⡙","⠈⠩","⠀⢙","⠀⡙","⠀⠩","⠀⢘","⠀⡘","⠀⠨","⠀⢐","⠀⡐","⠀⠠","⠀⢀","⠀⡀", }
    };

    Spinner static const dots13 = { 80, // dots13
        { "⣼","⣹","⢻","⠿","⡟","⣏","⣧","⣶", }
    };

    Spinner static const dots14 = { 80, // dots14
        { "⠉⠉","⠈⠙","⠀⠹","⠀⢸","⠀⣰","⢀⣠","⣀⣀","⣄⡀","⣆⠀","⡇⠀","⠏⠀","⠋⠁", }
    };

    Spinner static const dots8Bit = { 80, // dots8Bit
        { "⠀","⠁","⠂","⠃","⠄","⠅","⠆","⠇","⡀","⡁","⡂","⡃","⡄","⡅","⡆","⡇","⠈","⠉","⠊","⠋","⠌","⠍","⠎","⠏","⡈","⡉","⡊","⡋","⡌","⡍","⡎","⡏","⠐","⠑","⠒","⠓","⠔","⠕","⠖","⠗","⡐","⡑","⡒","⡓","⡔","⡕","⡖","⡗","⠘","⠙","⠚","⠛","⠜","⠝","⠞","⠟","⡘","⡙","⡚","⡛","⡜","⡝","⡞","⡟","⠠","⠡","⠢","⠣","⠤","⠥","⠦","⠧","⡠","⡡","⡢","⡣","⡤","⡥","⡦","⡧","⠨","⠩","⠪","⠫","⠬","⠭","⠮","⠯","⡨","⡩","⡪","⡫","⡬","⡭","⡮","⡯","⠰","⠱","⠲","⠳","⠴","⠵","⠶","⠷","⡰","⡱","⡲","⡳","⡴","⡵","⡶","⡷","⠸","⠹","⠺","⠻","⠼","⠽","⠾","⠿","⡸","⡹","⡺","⡻","⡼","⡽","⡾","⡿","⢀","⢁","⢂","⢃","⢄","⢅","⢆","⢇","⣀","⣁","⣂","⣃","⣄","⣅","⣆","⣇","⢈","⢉","⢊","⢋","⢌","⢍","⢎","⢏","⣈","⣉","⣊","⣋","⣌","⣍","⣎","⣏","⢐","⢑","⢒","⢓","⢔","⢕","⢖","⢗","⣐","⣑","⣒","⣓","⣔","⣕","⣖","⣗","⢘","⢙","⢚","⢛","⢜","⢝","⢞","⢟","⣘","⣙","⣚","⣛","⣜","⣝","⣞","⣟","⢠","⢡","⢢","⢣","⢤","⢥","⢦","⢧","⣠","⣡","⣢","⣣","⣤","⣥","⣦","⣧","⢨","⢩","⢪","⢫","⢬","⢭","⢮","⢯","⣨","⣩","⣪","⣫","⣬","⣭","⣮","⣯","⢰","⢱","⢲","⢳","⢴","⢵","⢶","⢷","⣰","⣱","⣲","⣳","⣴","⣵","⣶","⣷","⢸","⢹","⢺","⢻","⢼","⢽","⢾","⢿","⣸","⣹","⣺","⣻","⣼","⣽","⣾","⣿", }
    };

    Spinner static const dotsCircle = { 80, // dotsCircle
        { "⢎ ","⠎⠁","⠊⠑","⠈⠱"," ⡱","⢀⡰","⢄⡠","⢆⡀", }
    };

    Spinner static const sand = { 80, // sand
        { "⠁","⠂","⠄","⡀","⡈","⡐","⡠","⣀","⣁","⣂","⣄","⣌","⣔","⣤","⣥","⣦","⣮","⣶","⣷","⣿","⡿","⠿","⢟","⠟","⡛","⠛","⠫","⢋","⠋","⠍","⡉","⠉","⠑","⠡","⢁", }
    };

    Spinner static const line = { 130, // line
        { "-","\\","|","/", }
    };

    Spinner static const line2 = { 100, // line2
        { "⠂","-","–","—","–","-", }
    };

    Spinner static const pipe = { 100, // pipe
        { "┤","┘","┴","└","├","┌","┬","┐", }
    };

    Spinner static const simpleDots = { 400, // simpleDots
        { ".  ",".. ","...","   ", }
    };

    Spinner static const simpleDotsScrolling = { 200, // simpleDotsScrolling
        { ".  ",".. ","..."," ..","  .","   ", }
    };

    Spinner static const star = { 70, // star
        { "✶","✸","✹","✺","✹","✷", }
    };

    Spinner static const star2 = { 80, // star2
        { "+","x","*", }
    };

    Spinner static const flip = { 70, // flip
        { "_","_","_","-","`","`","'","´","-","_","_","_", }
    };

    Spinner static const hamburger = { 100, // hamburger
        { "☱","☲","☴", }
    };

    Spinner static const growVertical = { 120, // growVertical
        { "▁","▃","▄","▅","▆","▇","▆","▅","▄","▃", }
    };

    Spinner static const growHorizontal = { 120, // growHorizontal
        { "▏","▎","▍","▌","▋","▊","▉","▊","▋","▌","▍","▎", }
    };

    Spinner static const balloon = { 140, // balloon
        { " ",".","o","O","@","*"," ", }
    };

    Spinner static const balloon2 = { 120, // balloon2
        { ".","o","O","°","O","o",".", }
    };

    Spinner static const noise = { 100, // noise
        { "▓","▒","░", }
    };

    Spinner static const bounce = { 120, // bounce
        { "⠁","⠂","⠄","⠂", }
    };

    Spinner static const boxBounce = { 120, // boxBounce
        { "▖","▘","▝","▗", }
    };

    Spinner static const boxBounce2 = { 100, // boxBounce2
        { "▌","▀","▐","▄", }
    };

    Spinner static const triangle = { 50, // triangle
        { "◢","◣","◤","◥", }
    };

    Spinner static const binary = { 80, // binary
        { "010010","001100","100101","111010","111101","010111","101011","111000","110011","110101", }
    };

    Spinner static const arc = { 100, // arc
        { "◜","◠","◝","◞","◡","◟", }
    };

    Spinner static const circle = { 120, // circle
        { "◡","⊙","◠", }
    };

    Spinner static const squareCorners = { 180, // squareCorners
        { "◰","◳","◲","◱", }
    };

    Spinner static const circleQuarters = { 120, // circleQuarters
        { "◴","◷","◶","◵", }
    };

    Spinner static const circleHalves = { 50, // circleHalves
        { "◐","◓","◑","◒", }
    };

    Spinner static const squish = { 100, // squish
        { "╫","╪", }
    };

    Spinner static const toggle = { 250, // toggle
        { "⊶","⊷", }
    };

    Spinner static const toggle2 = { 80, // toggle2
        { "▫","▪", }
    };

    Spinner static const toggle3 = { 120, // toggle3
        { "□","■", }
    };

    Spinner static const toggle4 = { 100, // toggle4
        { "■","□","▪","▫", }
    };

    Spinner static const toggle5 = { 100, // toggle5
        { "▮","▯", }
    };

    Spinner static const toggle6 = { 300, // toggle6
        { "ဝ","၀", }
    };

    Spinner static const toggle7 = { 80, // toggle7
        { "⦾","⦿", }
    };

    Spinner static const toggle8 = { 100, // toggle8
        { "◍","◌", }
    };

    Spinner static const toggle9 = { 100, // toggle9
        { "◉","◎", }
    };

    Spinner static const toggle10 = { 100, // toggle10
        { "㊂","㊀","㊁", }
    };

    Spinner static const toggle11 = { 50, // toggle11
        { "⧇","⧆", }
    };

    Spinner static const toggle12 = { 120, // toggle12
        { "☗","☖", }
    };

    Spinner static const toggle13 = { 80, // toggle13
        { "=","*","-", }
    };

    Spinner static const arrow = { 100, // arrow
        { "←","↖","↑","↗","→","↘","↓","↙", }
    };

    Spinner static const arrow2 = { 80, // arrow2
        { "⬆️ ","↗️ ","➡️ ","↘️ ","⬇️ ","↙️ ","⬅️ ","↖️ ", }
    };

    Spinner static const arrow3 = { 120, // arrow3
        { "▹▹▹▹▹","▸▹▹▹▹","▹▸▹▹▹","▹▹▸▹▹","▹▹▹▸▹","▹▹▹▹▸", }
    };

    Spinner static const bouncingBar = { 80, // bouncingBar
        { "[    ]","[=   ]","[==  ]","[=== ]","[====]","[ ===]","[  ==]","[   =]","[    ]","[   =]","[  ==]","[ ===]","[====]","[=== ]","[==  ]","[=   ]", }
    };

    Spinner static const bouncingBall = { 80, // bouncingBall
        { "( ●    )","(  ●   )","(   ●  )","(    ● )","(     ●)","(    ● )","(   ●  )","(  ●   )","( ●    )","(●     )", }
    };

    Spinner static const smiley = { 200, // smiley
        { "😄 ","😝 ", }
    };

    Spinner static const monkey = { 300, // monkey
        { "🙈 ","🙈 ","🙉 ","🙊 ", }
    };

    Spinner static const hearts = { 100, // hearts
        { "💛 ","💙 ","💜 ","💚 ","❤️ ", }
    };

    Spinner static const clock = { 100, // clock
        { "🕛 ","🕐 ","🕑 ","🕒 ","🕓 ","🕔 ","🕕 ","🕖 ","🕗 ","🕘 ","🕙 ","🕚 ", }
    };

    Spinner static const earth = { 180, // earth
        { "🌍 ","🌎 ","🌏 ", }
    };

    Spinner static const material = { 17, // material
        { "█▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","██▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","███▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","████▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","██████▁▁▁▁▁▁▁▁▁▁▁▁▁▁","██████▁▁▁▁▁▁▁▁▁▁▁▁▁▁","███████▁▁▁▁▁▁▁▁▁▁▁▁▁","████████▁▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","██████████▁▁▁▁▁▁▁▁▁▁","███████████▁▁▁▁▁▁▁▁▁","█████████████▁▁▁▁▁▁▁","██████████████▁▁▁▁▁▁","██████████████▁▁▁▁▁▁","▁██████████████▁▁▁▁▁","▁██████████████▁▁▁▁▁","▁██████████████▁▁▁▁▁","▁▁██████████████▁▁▁▁","▁▁▁██████████████▁▁▁","▁▁▁▁█████████████▁▁▁","▁▁▁▁██████████████▁▁","▁▁▁▁██████████████▁▁","▁▁▁▁▁██████████████▁","▁▁▁▁▁██████████████▁","▁▁▁▁▁██████████████▁","▁▁▁▁▁▁██████████████","▁▁▁▁▁▁██████████████","▁▁▁▁▁▁▁█████████████","▁▁▁▁▁▁▁█████████████","▁▁▁▁▁▁▁▁████████████","▁▁▁▁▁▁▁▁████████████","▁▁▁▁▁▁▁▁▁███████████","▁▁▁▁▁▁▁▁▁███████████","▁▁▁▁▁▁▁▁▁▁██████████","▁▁▁▁▁▁▁▁▁▁██████████","▁▁▁▁▁▁▁▁▁▁▁▁████████","▁▁▁▁▁▁▁▁▁▁▁▁▁███████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁██████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█████","█▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁████","██▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁███","██▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁███","███▁▁▁▁▁▁▁▁▁▁▁▁▁▁███","████▁▁▁▁▁▁▁▁▁▁▁▁▁▁██","█████▁▁▁▁▁▁▁▁▁▁▁▁▁▁█","█████▁▁▁▁▁▁▁▁▁▁▁▁▁▁█","██████▁▁▁▁▁▁▁▁▁▁▁▁▁█","████████▁▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","█████████▁▁▁▁▁▁▁▁▁▁▁","███████████▁▁▁▁▁▁▁▁▁","████████████▁▁▁▁▁▁▁▁","████████████▁▁▁▁▁▁▁▁","██████████████▁▁▁▁▁▁","██████████████▁▁▁▁▁▁","▁██████████████▁▁▁▁▁","▁██████████████▁▁▁▁▁","▁▁▁█████████████▁▁▁▁","▁▁▁▁▁████████████▁▁▁","▁▁▁▁▁████████████▁▁▁","▁▁▁▁▁▁███████████▁▁▁","▁▁▁▁▁▁▁▁█████████▁▁▁","▁▁▁▁▁▁▁▁█████████▁▁▁","▁▁▁▁▁▁▁▁▁█████████▁▁","▁▁▁▁▁▁▁▁▁█████████▁▁","▁▁▁▁▁▁▁▁▁▁█████████▁","▁▁▁▁▁▁▁▁▁▁▁████████▁","▁▁▁▁▁▁▁▁▁▁▁████████▁","▁▁▁▁▁▁▁▁▁▁▁▁███████▁","▁▁▁▁▁▁▁▁▁▁▁▁███████▁","▁▁▁▁▁▁▁▁▁▁▁▁▁███████","▁▁▁▁▁▁▁▁▁▁▁▁▁███████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁████","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁███","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁███","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁██","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁██","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁██","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁","▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁", }
    };

    Spinner static const moon = { 80, // moon
        { "🌑 ","🌒 ","🌓 ","🌔 ","🌕 ","🌖 ","🌗 ","🌘 ", }
    };

    Spinner static const runner = { 140, // runner
        { "🚶 ","🏃 ", }
    };

    Spinner static const pong = { 80, // pong
        { "▐⠂       ▌","▐⠈       ▌","▐ ⠂      ▌","▐ ⠠      ▌","▐  ⡀     ▌","▐  ⠠     ▌","▐   ⠂    ▌","▐   ⠈    ▌","▐    ⠂   ▌","▐    ⠠   ▌","▐     ⡀  ▌","▐     ⠠  ▌","▐      ⠂ ▌","▐      ⠈ ▌","▐       ⠂▌","▐       ⠠▌","▐       ⡀▌","▐      ⠠ ▌","▐      ⠂ ▌","▐     ⠈  ▌","▐     ⠂  ▌","▐    ⠠   ▌","▐    ⡀   ▌","▐   ⠠    ▌","▐   ⠂    ▌","▐  ⠈     ▌","▐  ⠂     ▌","▐ ⠠      ▌","▐ ⡀      ▌","▐⠠       ▌", }
    };

    Spinner static const shark = { 120, // shark
        { "▐|\\____________▌","▐_|\\___________▌","▐__|\\__________▌","▐___|\\_________▌","▐____|\\________▌","▐_____|\\_______▌","▐______|\\______▌","▐_______|\\_____▌","▐________|\\____▌","▐_________|\\___▌","▐__________|\\__▌","▐___________|\\_▌","▐____________|\\▌","▐____________/|▌","▐___________/|_▌","▐__________/|__▌","▐_________/|___▌","▐________/|____▌","▐_______/|_____▌","▐______/|______▌","▐_____/|_______▌","▐____/|________▌","▐___/|_________▌","▐__/|__________▌","▐_/|___________▌","▐/|____________▌", }
    };

    Spinner static const dqpb = { 100, // dqpb
        { "d","q","p","b", }
    };

    Spinner static const weather = { 100, // weather
        { "☀️ ","☀️ ","☀️ ","🌤 ","⛅️ ","🌥 ","☁️ ","🌧 ","🌨 ","🌧 ","🌨 ","🌧 ","🌨 ","⛈ ","🌨 ","🌧 ","🌨 ","☁️ ","🌥 ","⛅️ ","🌤 ","☀️ ","☀️ ", }
    };

    Spinner static const christmas = { 400, // christmas
        { "🌲","🎄", }
    };

    Spinner static const grenade = { 80, // grenade
        { "،  ","′  "," ´ "," ‾ ","  ⸌","  ⸊","  |","  ⁎","  ⁕"," ෴ ","  ⁓","   ","   ","   ", }
    };

    Spinner static const point = { 125, // point
        { "∙∙∙","●∙∙","∙●∙","∙∙●","∙∙∙", }
    };

    Spinner static const layer = { 150, // layer
        { "-","=","≡", }
    };

    Spinner static const betaWave = { 80, // betaWave
        { "ρββββββ","βρβββββ","ββρββββ","βββρβββ","ββββρββ","βββββρβ","ββββββρ", }
    };

    Spinner static const fingerDance = { 160, // fingerDance
        { "🤘 ","🤟 ","🖖 ","✋ ","🤚 ","👆 ", }
    };

    Spinner static const fistBump = { 80, // fistBump
        { "🤜　　　　🤛 ","🤜　　　　🤛 ","🤜　　　　🤛 ","　🤜　　🤛　 ","　　🤜🤛　　 ","　🤜✨🤛　　 ","🤜　✨　🤛　 ", }
    };

    Spinner static const soccerHeader = { 80, // soccerHeader
        { " 🧑⚽️       🧑 ","🧑  ⚽️      🧑 ","🧑   ⚽️     🧑 ","🧑    ⚽️    🧑 ","🧑     ⚽️   🧑 ","🧑      ⚽️  🧑 ","🧑       ⚽️🧑  ","🧑      ⚽️  🧑 ","🧑     ⚽️   🧑 ","🧑    ⚽️    🧑 ","🧑   ⚽️     🧑 ","🧑  ⚽️      🧑 ", }
    };

    Spinner static const mindblown = { 160, // mindblown
        { "😐 ","😐 ","😮 ","😮 ","😦 ","😦 ","😧 ","😧 ","🤯 ","💥 ","✨ ","　 ","　 ","　 ", }
    };

    Spinner static const speaker = { 160, // speaker
        { "🔈 ","🔉 ","🔊 ","🔉 ", }
    };

    Spinner static const orangePulse = { 100, // orangePulse
        { "🔸 ","🔶 ","🟠 ","🟠 ","🔶 ", }
    };

    Spinner static const bluePulse = { 100, // bluePulse
        { "🔹 ","🔷 ","🔵 ","🔵 ","🔷 ", }
    };

    Spinner static const orangeBluePulse = { 100, // orangeBluePulse
        { "🔸 ","🔶 ","🟠 ","🟠 ","🔶 ","🔹 ","🔷 ","🔵 ","🔵 ","🔷 ", }
    };

    Spinner static const timeTravel = { 100, // timeTravel
        { "🕛 ","🕚 ","🕙 ","🕘 ","🕗 ","🕖 ","🕕 ","🕔 ","🕓 ","🕒 ","🕑 ","🕐 ", }
    };

    Spinner static const aesthetic = { 80, // aesthetic
        { "▰▱▱▱▱▱▱","▰▰▱▱▱▱▱","▰▰▰▱▱▱▱","▰▰▰▰▱▱▱","▰▰▰▰▰▱▱","▰▰▰▰▰▰▱","▰▰▰▰▰▰▰","▰▱▱▱▱▱▱", }
    };

    Spinner static const dwarfFortress = { 80, // dwarfFortress
        { " ██████£££  ","☺██████£££  ","☺██████£££  ","☺▓█████£££  ","☺▓█████£££  ","☺▒█████£££  ","☺▒█████£££  ","☺░█████£££  ","☺░█████£££  ","☺ █████£££  "," ☺█████£££  "," ☺█████£££  "," ☺▓████£££  "," ☺▓████£££  "," ☺▒████£££  "," ☺▒████£££  "," ☺░████£££  "," ☺░████£££  "," ☺ ████£££  ","  ☺████£££  ","  ☺████£££  ","  ☺▓███£££  ","  ☺▓███£££  ","  ☺▒███£££  ","  ☺▒███£££  ","  ☺░███£££  ","  ☺░███£££  ","  ☺ ███£££  ","   ☺███£££  ","   ☺███£££  ","   ☺▓██£££  ","   ☺▓██£££  ","   ☺▒██£££  ","   ☺▒██£££  ","   ☺░██£££  ","   ☺░██£££  ","   ☺ ██£££  ","    ☺██£££  ","    ☺██£££  ","    ☺▓█£££  ","    ☺▓█£££  ","    ☺▒█£££  ","    ☺▒█£££  ","    ☺░█£££  ","    ☺░█£££  ","    ☺ █£££  ","     ☺█£££  ","     ☺█£££  ","     ☺▓£££  ","     ☺▓£££  ","     ☺▒£££  ","     ☺▒£££  ","     ☺░£££  ","     ☺░£££  ","     ☺ £££  ","      ☺£££  ","      ☺£££  ","      ☺▓££  ","      ☺▓££  ","      ☺▒££  ","      ☺▒££  ","      ☺░££  ","      ☺░££  ","      ☺ ££  ","       ☺££  ","       ☺££  ","       ☺▓£  ","       ☺▓£  ","       ☺▒£  ","       ☺▒£  ","       ☺░£  ","       ☺░£  ","       ☺ £  ","        ☺£  ","        ☺£  ","        ☺▓  ","        ☺▓  ","        ☺▒  ","        ☺▒  ","        ☺░  ","        ☺░  ","        ☺   ","        ☺  &","        ☺ ☼&","       ☺ ☼ &","       ☺☼  &","      ☺☼  & ","      ‼   & ","     ☺   &  ","    ‼    &  ","   ☺    &   ","  ‼     &   "," ☺     &    ","‼      &    ","      &     ","      &     ","     &   ░  ","     &   ▒  ","    &    ▓  ","    &    £  ","   &    ░£  ","   &    ▒£  ","  &     ▓£  ","  &     ££  "," &     ░££  "," &     ▒££  ","&      ▓££  ","&      £££  ","      ░£££  ","      ▒£££  ","      ▓£££  ","      █£££  ","     ░█£££  ","     ▒█£££  ","     ▓█£££  ","     ██£££  ","    ░██£££  ","    ▒██£££  ","    ▓██£££  ","    ███£££  ","   ░███£££  ","   ▒███£££  ","   ▓███£££  ","   ████£££  ","  ░████£££  ","  ▒████£££  ","  ▓████£££  ","  █████£££  "," ░█████£££  "," ▒█████£££  "," ▓█████£££  "," ██████£££  "," ██████£££  ", }
    };

} // namespace ut::spinners

