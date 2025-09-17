package com.compiler.lexer;

import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

import com.compiler.lexer.nfa.*;
import com.compiler.lexer.dfa.*;
import com.compiler.lexer.regex.*;

/**
 * 1 - From an arbitrary regex list, convert them into a singular NFA using epsilon transitions in an
 * 'or'-like structure
 * 2 - Modify <States> so it may indicate what expression it finals to.
 * 3 - Constructor shall get token identifier from the user
 */
public class Tokenizer {

    private NFA initialVersion;
    private DFA currentRepresentation;

    /**
     * 
     * @param regexs - A {@link Map} containing the tokens id and regex
     */
    public Tokenizer( Map<String,String> regexs ){
        // List of individual NFAs for each token
        this.currentRepresentation = null;
        List<NFA> temp = new LinkedList<>();
        RegexParser parser = new RegexParser();
        NFA tempNFA;

        State start = new State();

        for ( Map.Entry<String,String> token : regexs.entrySet() ) {
            // Obtain plain NFA from tokens regex
            tempNFA = parser.parse( token.getValue() );
            // Update this NFA end state to have the tokens identifier
            tempNFA.getEndState().setTokenID( token.getKey() );
            temp.add(tempNFA); // Maybe unnecesary ?
            start.addTransition( new Transition(null, tempNFA.getStartState() ) );
        }
        this.initialVersion = new NFA(start);
    }

    public void buildDFA( Set<Character> alphabet ){
        this.currentRepresentation = NfaToDfaConverter.convertNfaToDfa(initialVersion, alphabet);
    }

    public List<Token> tokenize(String input) {
        List<Token> tokens = new LinkedList<>();

        // Ensure DFA has been already built
        if (this.currentRepresentation == null) {
            throw new IllegalArgumentException("No DFA has been built yet; Call buildDfa(alphabet) before trying to tokenize.");
        }

        int position = 0;
        int currentPos,lastFinalPos;
        int length = input.length();
        char currentChar;
        DfaState currentState, lastFinalState;
        //for( int position = 0; position<length; position = lastFinalPos + 1){
        while (position < length) {
            currentPos = position;
            currentState = currentRepresentation.startState;

            lastFinalState = null;
            lastFinalPos = -1;

            while (currentPos < length) {
                currentChar = input.charAt(currentPos);
                currentState = currentState.getTransition(currentChar);

                if (currentState == null){
                    break;
                }
                if (currentState.isFinal()) {
                    lastFinalState = currentState;
                    lastFinalPos = currentPos;
                }

                currentPos++;
            }

            // No valid token was recognized
            if (lastFinalState == null) {
                throw new RuntimeException("Lexical error at position " + position + ": '" + input.charAt(position) + "'");
            }

            // Recognize longest found token
            tokens.add( new Token( lastFinalState.getTokenID(),
                input.substring(position, lastFinalPos + 1) ));
            position = lastFinalPos + 1;
            //}
        }

        return tokens;
    }
    
    public static void main(String[] args) {
        Map<String, String> regexMap = new HashMap<>();
        regexMap.put("IF", "if");
        regexMap.put("ID", "a|b|c");
        regexMap.put("NUM", "0|1|2");

        Tokenizer tokenizer = new Tokenizer(regexMap);
        

        Set<Character> alphabet = new HashSet<>(Arrays.asList('i', 'f', 'a', 'b', 'c', '0', '1', '2', ' '));
        tokenizer.buildDFA( alphabet );

        // Prueba 1: palabra "if"
        List<Token> tokens1 = tokenizer.tokenize("if");
        for (Token t : tokens1){
            System.out.println(t);
        }

        // Prueba 2: secuencia "a 1 b"
        List<Token> tokens2 = tokenizer.tokenize("a 1 b");
        for (Token t : tokens2){
            System.out.println(t);
        }
    }
}
