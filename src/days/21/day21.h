#include <stdlib.h>

char *n_keypad[11][11][2] = { {  {"A"},
  {"^<A"},
  {"^A"},
  {">^A", "^>A"},
  {"^^<A"},
  {"^^A"},
  {">^^A", "^^>A"},
  {"^^^<A"},
  {"^^^A"},
  {"^^^>A", ">^^^A"},
  {">A"},
 },
 {  {">vA"},
  {"A"},
  {">A"},
  {">>A"},
  {"^A"},
  {">^A", "^>A"},
  {">>^A", "^>>A"},
  {"^^A"},
  {">^^A", "^^>A"},
  {"^^>>A", ">>^^A"},
  {">>vA"},
 },
 {  {"vA"},
  {"<A"},
  {"A"},
  {">A"},
  {"^<A", "<^A"},
  {"^A"},
  {">^A", "^>A"},
  {"^^<A", "<^^A"},
  {"^^A"},
  {">^^A", "^^>A"},
  {"v>A", ">vA"},
 },
 {  {"v<A", "<vA"},
  {"<<A"},
  {"<A"},
  {"A"},
  {"<<^A", "^<<A"},
  {"^<A", "<^A"},
  {"^A"},
  {"^^<<A", "<<^^A"},
  {"^^<A", "<^^A"},
  {"^^A"},
  {"vA"},
 },
 {  {">vvA", "v>vA"},
  {"vA"},
  {"v>A", ">vA"},
  {">>vA", "v>>A"},
  {"A"},
  {">A"},
  {">>A"},
  {"^A"},
  {">^A", "^>A"},
  {">>^A", "^>>A"},
  {">>vvA"},
 },
 {  {"vvA"},
  {"v<A", "<vA"},
  {"vA"},
  {"v>A", ">vA"},
  {"<A"},
  {"A"},
  {">A"},
  {"^<A", "<^A"},
  {"^A"},
  {">^A", "^>A"},
  {"vv>A", ">vvA"},
 },
 {  {"vv<A", "<vvA"},
  {"<<vA", "v<<A"},
  {"v<A", "<vA"},
  {"vA"},
  {"<<A"},
  {"<A"},
  {"A"},
  {"<<^A", "^<<A"},
  {"^<A", "<^A"},
  {"^A"},
  {"vvA"},
 },
 {  {">vvvA", "v>vvA"},
  {"vvA"},
  {"vv>A", ">vvA"},
  {">>vvA", "vv>>A"},
  {"vA"},
  {"v>A", ">vA"},
  {">>vA", "v>>A"},
  {"A"},
  {">A"},
  {">>A"},
  {">>vvvA", "vv>>vA"},
 },
 {  {"vvvA"},
  {"vv<A", "<vvA"},
  {"vvA"},
  {"vv>A", ">vvA"},
  {"v<A", "<vA"},
  {"vA"},
  {"v>A", ">vA"},
  {"<A"},
  {"A"},
  {">A"},
  {">vvvA", "vvv>A"},
 },
 {  {"<vvvA", "vvv<A"},
  {"vv<<A", "<<vvA"},
  {"vv<A", "<vvA"},
  {"vvA"},
  {"<<vA", "v<<A"},
  {"v<A", "<vA"},
  {"vA"},
  {"<<A"},
  {"<A"},
  {"A"},
  {"vvvA"},
 },
 {  {"<A"},
  {"^<<A"},
  {"^<A"},
  {"^A"},
  {"^^<<A"},
  {"^^<A"},
  {"^^A"},
  {"^^^<<A" },
  {"<^^^A"},
  {"^^^A"},
  {"A"},
 },
};


char *d_keypad[5][5] = { 
 { "A", "vA", "v<A", "v>A", ">A" },
 { "^A", "A", "<A", ">A", ">^A", },
 { ">^A", ">A", "A", ">>A", ">>^A", },
 { "^<A", "<A", "<<A", "A", "^A", },
 { "<A", "v<A", "v<<A", "vA", "A", },
};
