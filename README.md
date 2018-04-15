# C4Bot
A C++ [connect four](https://playground.riddles.io/competitions/four-in-a-row) starter bot for the [riddles.io](https://www.riddles.io) platform. Using the starter template from Aswin van Woudenberg.

## Algorithm
This bot uses the Negamax (minimax search) that relies on the zero-sum property of a two player game.
https://en.wikipedia.org/wiki/Negamax
Some optimisations include filtering of useless threats (can be enabled by using the function filterThreats()) and move ordering.

