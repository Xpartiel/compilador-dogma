package com.compiler.lexer.regex;

import java.util.Map;
import java.util.HashMap;

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * Utility class for regular expression parsing using the Shunting Yard
 * algorithm.
 * <p>
 * Provides methods to preprocess regular expressions by inserting explicit
 * concatenation operators, and to convert infix regular expressions to postfix
 * notation for easier parsing and NFA construction.
 */
/**
 * Utility class for regular expression parsing using the Shunting Yard
 * algorithm.
 */
public class ShuntingYard {

    /**
     * Default constructor for ShuntingYard.
     */
    public ShuntingYard() {
        // TODO: Implement constructor if needed
    }

    /**
     * Inserts the explicit concatenation operator ('·') into the regular
     * expression according to standard rules. This makes implicit
     * concatenations explicit, simplifying later parsing.
     *
     * @param regex Input regular expression (may have implicit concatenation).
     * @return Regular expression with explicit concatenation operators.
     */
    public static String insertConcatenationOperator(String regex) {
        /*
            Pseudocode:
            For each character in regex:
                - Append current character to output
                - If not at end of string:
                    - Check if current and next character form an implicit concatenation
                    - If so, append '·' to output
            Return output as string
         */
        StringBuilder res = new StringBuilder();
        char charAct,charNext;
        int largo = regex.length();
        // For each character within the regular expression...
        for( int i=0; i<largo ; i++ ){
            charAct = regex.charAt(i);
            res.append( charAct );//Add current character
            if( (largo-1) <= i ){//End if its the last char
                break;
            }
            charNext = regex.charAt(i+1);
            if( isOperand(charAct) || isUnary(charAct) || charAct==')' ){
                if( isOperand( charNext ) || charNext=='('){
                    res.append(".");
        }   }   }
        return res.toString();
    }

    /**
     * Determines if the given character is an operand (not an operator or
     * parenthesis).
     *
     * @param c Character to evaluate.
     * @return true if it is an operand, false otherwise.
     */
    private static boolean isOperand(char c) {
        /*
        Pseudocode:
        Return true if c is not one of: '|', '*', '?', '+', '(', ')', '·'
         */
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

    /**
     * Auxiliary function that cleanly determines if a given character is to be
     * considered a unary operation or not
     * @param c - {@code char} to be analyzed
     * @return - {@code true} if it fids any of the following characters:
     * <ul>
     *  <li>+</li>
     *  <li>*</li>
     *  <li>?</li>
     * </ul>
     * - {@code false} otherwise
     */
    public static boolean isUnary( char c ){
        if ( isOperand(c) ){
            return false;
        }
        switch (c) {
            case '*':
            case '+':
            case '?':
                return true;
            default:
                return false;
        }
    }

    /**
     * Converts an infix regular expression to postfix notation using the
     * Shunting Yard algorithm. This is useful for constructing NFAs from
     * regular expressions.
     *
     * @param infixRegex Regular expression in infix notation.
     * @return Regular expression in postfix notation.
     */
    public static String toPostfix(String infixRegex) {
        /*
        Pseudocode:
        1. Define operator precedence map
        2. Preprocess regex to insert explicit concatenation operators
        3. For each character in regex:
            - If operand: append to output
            - If '(': push to stack
            - If ')': pop operators to output until '(' is found
            - If operator: pop operators with higher/equal precedence, then push current operator
        4. After loop, pop remaining operators to output
        5. Return output as string
        */
        Map<Character, Integer> operators = new HashMap<>();
        operators.put('*', 4);
        operators.put('+', 3);
        operators.put('.', 2);
        operators.put('|', 1);
        operators.put('(', 0);operators.put(')', 0);

        //variables auxiliares
        Deque<Character> pila = new ArrayDeque<>();
        String concats = insertConcatenationOperator(infixRegex);
        StringBuilder res = new StringBuilder();
        int length = concats.length(), hierarchy;
        char charAct,popAct;
        for (int i=0; i<length; i++) {
            charAct = concats.charAt(i);
            if( isOperand(charAct) ){
                res.append(charAct);
                continue;
            }
            if( charAct=='('){
                pila.push(charAct);
            }else if( charAct==')'){
                while( (!pila.isEmpty()) && ((popAct = pila.pop()) != '(') ){
                    res.append( popAct );
            }   }
            else{
                hierarchy = operators.get(charAct);
                while( (!pila.isEmpty()) && (hierarchy<=operators.get(pila.peek()))){
                    res.append( pila.pop() );
                }
                pila.push(charAct);
        }   }
        //concatenamos todo lo que reste de la pila de operadores.
        while (!pila.isEmpty()) {
            res.append( pila.pop() );
        }
        return res.toString();
    }
}
