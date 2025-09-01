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
     * <p>
     * Pseudocode: Convert infix to postfix, then build NFA from postfix
     * </p>
     * @param infixRegex The regular expression in infix notation.
     * @return The constructed NFA.
     */
    public NFA parse(String infixRegex) {
        return buildNfaFromPostfix( ShuntingYard.toPostfix(infixRegex) );
    }

    /**
     * Builds an NFA from a postfix regular expression.
     * <p>
     * Pseudocode: For each char in postfix, handle operators and operands using a stack
     * </p>
     * @param postfixRegex The regular expression in postfix notation.
     * @return The constructed NFA.
     */
    private NFA buildNfaFromPostfix(String postfixRegex) {
        Stack<NFA> stack = new Stack<>();
        char charAct;
        int length = postfixRegex.length();
        for (int i=0; i<length; i++) {
            charAct = postfixRegex.charAt(i);
            switch (charAct) {
                case '+':
                    handlePlus(stack);
                    break;
                case '*':
                    handleKleeneStar(stack);
                    break;
                case '?':
                    handleOptional(stack);
                    break;
                case '|':
                    handleUnion(stack);
                    break;
                case '.':
                    handleConcatenation(stack);
                    break;
                default:
                    stack.push( createNfaForCharacter(charAct) );
                    break;
        }   }
        return stack.pop();
    }

    /**
     * Handles the '?' operator (zero or one occurrence).
     * Pops an NFA from the stack and creates a new NFA that accepts zero or one occurrence.
     * <p>
     * Pseudocode: Pop NFA, create new start/end, add epsilon transitions for zero/one occurence
     * </p>
     * @param stack The NFA stack.
     */
    private void handleOptional(Stack<NFA> stack) {
        NFA top = stack.pop();
        NFA nfa = new NFA( new State(), new State() );

        // Connect new start with old start
        nfa.getStartState().addTransition( new Transition(null, top.getStartState()) );
        // Connect old end with new end
        top.getEndState().addTransition( new Transition(null, nfa.getEndState()) );
        // Directly connect new start and end 
        nfa.getStartState().addTransition( new Transition(null, nfa.getEndState()) );
        // Update old end flag as not final anymore
        top.getEndState().isFinal = false;

        // Push resulting automata to stack
        stack.push(nfa);
    }

    /**
     * Handles the '+' operator (one or more occurrences).
     * Pops an NFA from the stack and creates a new NFA that accepts one or more occurrences.
     * <p>
     * Pseudocode: Pop NFA, create new start/end, add transitions for one or more occurrence
     * </p>
     * @param stack The NFA stack.
     */
    private void handlePlus(Stack<NFA> stack) {
        NFA top = stack.pop();
        NFA res = new NFA( new State() , new State() );

        // Create transition from new start to old start
        res.getStartState().addTransition( new Transition(null, top.getStartState()) );
        
        // Create transition from old end to old start
        top.getEndState().addTransition( new Transition(null, top.getStartState()) );
        // Create transition from old end to new end
        top.getEndState().addTransition( new Transition(null, res.getEndState()) );
        // Update old end flag as not final anymore
        top.getEndState().isFinal = false;
        
        // Push resulting automata to stack
        stack.push(res);
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

        // Update left's end flag as not final anymore
        left.getEndState().isFinal = false;
        // Create transition from left's old end state to right's old start state
        left.getEndState().addTransition( new Transition(null, right.getStartState()) );

        // Push resulting automata to stack
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

        // Connect new start with old starts
        nStartState.addTransition( new Transition(null, left.getStartState()) );
        nStartState.addTransition( new Transition(null, right.getStartState()) );

        // Update old end flags as not final anymore
        left.getEndState().isFinal = false;
        right.getEndState().isFinal = false;

        // Conect old ends with new end
        left.getEndState().addTransition( new Transition(null, nEndState) );
        right.getEndState().addTransition( new Transition(null, nEndState) );

        // Push resulting automata to stack
        stack.add( new NFA(nStartState , nEndState) );
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

        // Connect old end with old start
        top.getEndState().addTransition( new Transition(null, top.getStartState()) );
        // Connect old end with new end
        top.getEndState().addTransition( new Transition(null, nEndState ) );
        // Update old end flag as not final anymore
        top.getEndState().isFinal = false;

        // Connect new start with new end
        nStartState.addTransition( new Transition(null, nEndState) );
        // Connect new start with old start
        nStartState.addTransition( new Transition(null, top.getStartState()) );
        
        // Push resulting automata to stack
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
}   }   }