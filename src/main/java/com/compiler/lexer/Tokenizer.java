package com.compiler.lexer;

import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;

import com.compiler.lexer.nfa.*;
import com.compiler.lexer.dfa.*;
import com.compiler.lexer.regex.*;

import com.compiler.lexer.nfa.State;

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
}
