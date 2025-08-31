package com.compiler.lexer.regex;

import java.util.Stack;

import com.compiler.lexer.nfa.NFA;
import com.compiler.lexer.nfa.State;
import com.compiler.lexer.nfa.Transition;

/**
 * RegexParser
 * -----------
 * This class provides functionality to convert infix regular expressions into nondeterministic finite automata (NFA)
 * using Thompson's construction algorithm. It supports standard regex operators: concatenation (·), union (|),
 * Kleene star (*), optional (?), and plus (+). The conversion process uses the Shunting Yard algorithm to transform
 * infix regex into postfix notation, then builds the corresponding NFA.
 *
 * Features:
 * - Parses infix regular expressions and converts them to NFA.
 * - Supports regex operators: concatenation, union, Kleene star, optional, plus.
 * - Implements Thompson's construction rules for NFA generation.
 *
 * Example usage:
 * <pre>
 *     RegexParser parser = new RegexParser();
 *     NFA nfa = parser.parse("a(b|c)*");
 * </pre>
 */
/**
 * Parses regular expressions and constructs NFAs using Thompson's construction.
 */
public class RegexParser {
    /**
     * Default constructor for RegexParser.
     */
        public RegexParser() {
            // TODO: Implement constructor if needed
        }

    /**
     * Converts an infix regular expression to an NFA.
     *
     * @param infixRegex The regular expression in infix notation.
     * @return The constructed NFA.
     */
    public NFA parse(String infixRegex) {
    // TODO: Implement parse
    // Pseudocode: Convert infix to postfix, then build NFA from postfix
    throw new UnsupportedOperationException("Not implemented");
    }

    /**
     * Builds an NFA from a postfix regular expression.
     *
     * @param postfixRegex The regular expression in postfix notation.
     * @return The constructed NFA.
     */
    private NFA buildNfaFromPostfix(String postfixRegex) {
    // TODO: Implement buildNfaFromPostfix
    // Pseudocode: For each char in postfix, handle operators and operands using a stack
    throw new UnsupportedOperationException("Not implemented");
    }

    /**
     * Handles the '?' operator (zero or one occurrence).
     * Pops an NFA from the stack and creates a new NFA that accepts zero or one occurrence.
     * @param stack The NFA stack.
     */
    private void handleOptional(Stack<NFA> stack) {
    // TODO: Implement handleOptional
    // Pseudocode: Pop NFA, create new start/end, add epsilon transitions for zero/one occurrence
    throw new UnsupportedOperationException("Not implemented");
    }

    /**
     * Handles the '+' operator (one or more occurrences).
     * Pops an NFA from the stack and creates a new NFA that accepts one or more occurrences.
     * @param stack The NFA stack.
     */
    private void handlePlus(Stack<NFA> stack) {
    // TODO: Implement handlePlus
    // Pseudocode: Pop NFA, create new start/end, add transitions for one or more occurrence
    throw new UnsupportedOperationException("Not implemented");
    }
    
    /**
     * Creates an NFA for a single character.
     * <p>
     * Pseudocode: Create start/end state, add transition for character
     * </p>
     * @param c The character to create an NFA for.
     * @return The constructed NFA.
     */
    private NFA createNfaForCharacter(char c) {
        // Create new automata with 2 blank states
        NFA nfa = new NFA( new State() , new State() );
        // Make proposed end state final
        nfa.getEndState().isFinal = true;
        // Conect start and end states with a symbol transition
        nfa.getStartState().addTransition( new Transition(c, nfa.getEndState()) );

        return nfa;
    }

    /**
     * Handles the concatenation operator (·).
     * Pops two NFAs from the stack and connects them in sequence.
     * <p>
     * Pseudocode: Pop two NFAs, connect end of first to start of second
     * </p>
     * @param stack The NFA stack.
     */
    private void handleConcatenation(Stack<NFA> stack) {
        NFA right = stack.pop();
        NFA left = stack.pop();

        // Remove final status to left's side end state
        left.getEndState().isFinal = false;
        // Create transition from left's old end state to right's old start state
        left.getEndState().addTransition( new Transition(null, right.getStartState()) );

        // Push resulting automata into stack
        stack.push( new NFA( left.getStartState() , right.getEndState() ) );
    }

    /**
     * Handles the union operator (|).
     * Pops two NFAs from the stack and creates a new NFA that accepts either.
     * <p>
     * Pseudocode: Pop two NFAs, create new start/end, add epsilon transitions for union
     * </p>
     * @param stack The NFA stack.
     */
    private void handleUnion(Stack<NFA> stack) {
        NFA left = stack.pop();
        NFA right = stack.pop();

        // Create new States
        State nStartState = new State();
        State nEndState = new State();
        nEndState.isFinal = true;

        // Connect new start with old starts
        nStartState.addTransition( new Transition(null, left.getStartState()) );
        nStartState.addTransition( new Transition(null, right.getStartState()) );

        // Remove final status to old ends
        left.getEndState().isFinal = false;
        right.getEndState().isFinal = false;

        // Conect old ends with new end
        left.getEndState().addTransition( new Transition(null, nEndState) );
        right.getEndState().addTransition( new Transition(null, nEndState) );

        // Push resulting automata
        stack.add( new NFA(nStartState, nEndState) );
    }

    /**
     * Handles the Kleene star operator (*).
     * Pops an NFA from the stack and creates a new NFA that accepts zero or more repetitions.
     * <p>
     * Pseudocode: Pop NFA, create new start/end, add transitions for zero or more repetitions
     * </p>
     * @param stack The NFA stack.
     */
    private void handleKleeneStar(Stack<NFA> stack) {
        NFA top = stack.pop();
        State nStartState = new State();

        State nEndState = new State();
        nEndState.isFinal = true;

        //Connect old end with old start
        top.getEndState().addTransition( new Transition(null, top.getStartState()) );
        //Connect old end with new end
        top.getEndState().addTransition( new Transition(null, nEndState ) );
        //Remove final status from old final
        top.getEndState().isFinal = false;

        //Connect new start with new end
        nStartState.addTransition( new Transition(null, nEndState) );
        //Connect new start with old start
        nStartState.addTransition( new Transition(null, top.getStartState()) );
        
        //Push resulting automata
        stack.push( new NFA( nStartState , nEndState) );
    }

    /**
     * Checks if a character is an operand (not an operator).
     * @param c The character to check.
     * @return True if the character is an operand, false if it is an operator.
     */
    private boolean isOperand(char c) {
        switch (c) {
            case '|':
            case '*':
            case '?':
            case '+':
            case '(':
            case ')':
            case '.':
                return false;
            default:
                return true;
        }
    }
}