package com.compiler.lexer;

import java.util.Set;
import java.util.HashSet;

import com.compiler.lexer.nfa.NFA;
import com.compiler.lexer.nfa.State;
import com.compiler.lexer.nfa.Transition;


/**
 * NfaSimulator
 * ------------
 * This class provides functionality to simulate a Non-deterministic Finite Automaton (NFA)
 * on a given input string. It determines whether the input string is accepted by the NFA by processing
 * each character and tracking the set of possible states, including those reachable via epsilon (ε) transitions.
 *
 * Simulation steps:
 * - Initialize the set of current states with the ε-closure of the NFA's start state.
 * - For each character in the input, compute the next set of states by following transitions labeled with that character,
 *   and include all states reachable via ε-transitions from those states.
 * - After processing the input, check if any of the current states is a final (accepting) state.
 *
 * The class also provides a helper method to compute the ε-closure of a given state, which is the set of all states
 * reachable from the given state using only ε-transitions.
 */
/**
 * Simulator for running input strings on an NFA.
 */
public class NfaSimulator {
    /**
     * Default constructor for NfaSimulator.
     */
        public NfaSimulator() {
            // TODO: Implement constructor if needed
        }

    /**
     * Simulates the NFA on the given input string.
     * Starts at the NFA's start state and processes each character, following transitions and epsilon closures.
     * If any final state is reached after processing the input, the string is accepted.
     *
     * @param nfa The NFA to simulate.
     * @param input The input string to test.
     * @return True if the input is accepted by the NFA, false otherwise.
     */
    public boolean simulate(NFA nfa, String input) {
        /*
         Pseudocode:
         1. Initialize currentStates with epsilon-closure of NFA start state
         2. For each character in input:
              - For each state in currentStates:
                  - For each transition:
                      - If transition symbol matches character:
                          - Add epsilon-closure of destination state to nextStates
              - Set currentStates to nextStates
         3. After input, if any state in currentStates is final, return true
         4. Otherwise, return false
        */
        //conjunto auxiliar.
        Set<State> currentStates = new HashSet<State>();
        //agrego la cerradura epsilon del estado inicial al conjunto.
        addEpsilonClosure( nfa.getStartState() , currentStates );
        int length = input.length();
        char charAct;
        Set<State> nextStates;

        // For each charecter in input...
        for (int i=0; i<length; i++){
            charAct = input.charAt(i);
            nextStates = new HashSet<>();
            // For each state in current states...
            for ( State state : currentStates ){
                // For each transition...
                for ( Transition transition : state.transitions ) {
                    if( transition.symbol != null && transition.symbol.charValue()==charAct ){
                        addEpsilonClosure( transition.toState , nextStates );
                    }
                }
            }
            currentStates = nextStates;
        }
        for (State state : currentStates) {
            if( state.isFinal() ){
                return true;
            }
        }
        return false;
    }

    /**
     * Computes the epsilon-closure: all states reachable from 'start' using only epsilon (null) transitions.
     *
     * @param start The starting state.
     * @param closureSet The set to accumulate reachable states.
     */
    private void addEpsilonClosure(State start, Set<State> closureSet) {
        /*
         Pseudocode:
         If start not in closureSet:
             - Add start to closureSet
             - For each transition in start:
                 - If transition symbol is null:
                     - Recursively add epsilon-closure of destination state
        */

        // Guard Clause: start is already on closure Set
        if( closureSet.contains(start) ){
            return;
        }
        // Add start to closure Set
        closureSet.add(start);
        // For each transition in start...
        for ( Transition transition : start.transitions ) {
            // Transition symbol is null...
            if( transition.symbol == null ){
                // Recursive call for epsilon-closure with destination states
                addEpsilonClosure( transition.toState , closureSet);
            }
        }
    }
}