#include <stdlib.h>

char *n_keypad[11][11] = {
    {
        "A",
        "^<A",
        "^A",
        "^>A",
        "^^<A",
        "^^A",
        "^^>A",
        "^^^<A",
        "^^^A",
        "^^^>A",
        ">A",
    },
    {
        ">vA",
        "A",
        ">A",
        ">>A",
        "^A",
        "^>A",
        "^>>A",
        "^^A",
        "^^>A",
        "^^>>A",
        ">>vA",
    },
    {
        "vA",
        "<A",
        "A",
        ">A",
        "<^A",
        "^A",
        "^>A",
        "<^^A",
        "^^A",
        "^^>A",
        "v>A",
    },
    {
        "<vA",
        "<<A",
        "<A",
        "A",
        "<<^A",
        "<^A",
        "^A",
        "<<^^A",
        "<^^A",
        "^^A",
        "vA",
    },
    {
        ">vvA",
        "vA",
        "v>A",
        "v>>A",
        "A",
        ">A",
        ">>A",
        "^A",
        "^>A",
        "^>>A",
        ">>vvA",
    },
    {
        "vvA",
        "<vA",
        "vA",
        "v>A",
        "<A",
        "A",
        ">A",
        "<^A",
        "^A",
        "^>A",
        "vv>A",
    },
    {
        "<vvA",
        "<<vA",
        "<vA",
        "vA",
        "<<A",
        "<A",
        "A",
        "<<^A",
        "<^A",
        "^A",
        "vvA",
    },
    {
        ">vvvA",
        "vvA",
        "vv>A",
        "vv>>A",
        "vA",
        "v>A",
        "v>>A",
        "A",
        ">A",
        ">>A",
        ">>vvvA",
    },
    {
        "vvvA",
        "<vvA",
        "vvA",
        "vv>A",
        "<vA",
        "vA",
        "v>A",
        "<A",
        "A",
        ">A",
        "vvv>A",
    },
    {
        "<vvvA",
        "<<vvA",
        "<vvA",
        "vvA",
        "<<vA",
        "<vA",
        "vA",
        "<<A",
        "<A",
        "A",
        "vvvA",
    },
    {
        "<A",
        "^<<A",
        "^<A",
        "^A",
        "^^<<A",
        "^^<A",
        "^^A",
        "^^^<<A",
        "<^^^A",
        "^^^A",
        "A",
    },
};

char *d_keypad[5][5] = {
    {
        "A",
        "vA",
        "v<A",
        "v>A",
        ">A",
    },
    {
        "^A",
        "A",
        "<A",
        ">A",
        "^>A",
    },
    {
        ">^A",
        ">A",
        "A",
        ">>A",
        ">>^A",
    },
    {
        "<^A",
        "<A",
        "<<A",
        "A",
        "^A",
    },
    {
        "<A",
        "<vA",
        "v<<A",
        "vA",
        "A",
    },
};