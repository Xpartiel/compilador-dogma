package com.compiler.lexer.nfa;

import java.util.LinkedList;
import java.util.List;

/**
 * Represents a state in a Non-deterministic Finite Automaton (NFA).
 * Each state has a unique identifier, a list of transitions to other states,
 * and a flag indicating whether it is a final (accepting) state.
 *
 * <p>
 * Fields:
 * <ul>
 *   <li>{@code id} - Unique identifier for the state.</li>
 *   <li>{@code transitions} - List of transitions from this state to others.</li>
 *   <li>{@code isFinal} - Indicates if this state is an accepting state.</li>
 * </ul>
 *
 *
 * <p>
 * The {@code nextId} static field is used to assign unique IDs to each state.
 * </p>
 */
public class State {
    private static int nextId = 0;
    /**
     * Unique identifier for this state.
     */
    public final int id;

    /**
     * List of transitions from this state to other states.
     */
    public List<Transition> transitions;

    /**
     * Indicates if this state is a final (accepting) state.
     */
    public boolean isFinal;

    /**
     * Constructs a new state with a unique identifier and no transitions.
     * The state is not final by default.
     */
    public State() {
        this.id = nextId;
        ++nextId;

        this.transitions = new LinkedList<>();
    }

    /**
     * Checks if this {@link State} is a final (accepting) state.
     * @return {@code true} if this state is final, {@code false} otherwise
     */
    public boolean isFinal() {
        return this.isFinal;
    }

    /**
     * Returns the states reachable from this state via epsilon transitions (symbol == null).
     * @return a list of states reachable by epsilon transitions
     */
    public List<State> getEpsilonTransitions() {
        List<State> res = new LinkedList<>();
        for (Transition transition: this.transitions) {
            if( transition.symbol == null ){
                res.add( transition.toState );
        }   }
        return res;
    }

    /**
     * Returns the states reachable from this state via a transition with the given symbol.
     * @param symbol the symbol for the transition
     * @return a list of states reachable by the given symbol
     */
    public List<State> getTransitions( char symbol ) {
        List<State> res = new LinkedList<>();
        for ( Transition transition : this.transitions ) {
            if( transition.symbol != null && transition.symbol.charValue() == symbol ){
                res.add(transition.toState);
        }   }
        return res;
    }

    /**
     * Auxiliary method for inserting new {@link Transition}s into already
     * existing {@link State}s
     * @param transition the {@link Transition} to be inserted.
     * <p>
     * This method doesn't have any mechanism to avoid duplicate {@link Transition}s
     * </p>
     */
    public void addTransition( Transition transition ){
        this.transitions.add(transition);
 
    }

    /**
     * Auxiliar method to check equality.
     * checks if @param obj is a State.
     * checks if the id´s @param obj is the same.
     * checks if both are final or not.
     * checks if they have the same transitions.
     * @param obj the element to compare.
     * @return {@code True} if they are equal, <code> false </code> in any other case.
     */
    @Override
    public boolean equals(Object obj){
        //case is instance of State
        if (obj instanceof State){
            State new_State=(State)obj;
            
            //case id
            if (this.id == new_State.id){

                //case final
                if (this.isFinal == new_State.isFinal){

                    //case transitions.
                    //same size.
                    if (this.transitions.size()==new_State.transitions.size()){
                     //for each to check all transitions.
                        for(Transition trans:this.transitions){
                            if(!new_State.transitions.contains(trans)){
                                return false;
                            }
                        }
                        //after check all elements
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * Auxiliary method to check if two States are equivalent.
     * @param obj the object to compare.
     * checks if @param obj is a State.
     * checks if both are final or not.
     * checks if they have the same transitions.
     * @return {@code True} if they are equivlalent, <code> false </code> in any other case.
     */
    public boolean equivalent(Object obj){
        //case is instance of State
        if (obj instanceof State){
            State new_State=(State)obj;
            //case final
            if (this.isFinal == new_State.isFinal){

            //case transitions.
            //same size.
                if (this.transitions.size()==new_State.transitions.size()){
                //for each to check all transitions.
                    for(Transition trans:this.transitions){
                        if(!new_State.transitions.contains(trans)){
                            return false;
                        }
                    }
                        //after check all elements
                        return true;
                }
            }
            
        }
        return false;
    }
}