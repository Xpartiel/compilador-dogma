package com.compiler.lexer;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;

import com.compiler.lexer.dfa.DFA;
import com.compiler.lexer.dfa.DfaState;
import com.compiler.lexer.nfa.NFA;
import com.compiler.lexer.nfa.State;
import com.compiler.lexer.nfa.Transition;

/**
 * NfaToDfaConverter
 * -----------------
 * This class provides a static method to convert a Non-deterministic Finite Automaton (NFA)
 * into a Deterministic Finite Automaton (DFA) using the standard subset construction algorithm.
 */
/**
 * Utility class for converting NFAs to DFAs using the subset construction algorithm.
 */
public class NfaToDfaConverter {
	/**
	 * Default constructor for NfaToDfaConverter.
	 */
		public NfaToDfaConverter() {
			// TODO: Implement constructor if needed
		}

	/**
	 * Converts an NFA to a DFA using the subset construction algorithm.
	 * Each DFA state represents a set of NFA states. Final states are marked if any NFA state in the set is final.
	 *
	 * @param nfa The input NFA
	 * @param alphabet The input alphabet (set of characters)
	 * @return The resulting DFA
	 */
	public static DFA convertNfaToDfa(NFA nfa, Set<Character> alphabet) {
		
		/*
		 Pseudocode:
		 1. Create initial DFA state from epsilon-closure of NFA start state
		 2. While there are unmarked DFA states:
			  - For each symbol in alphabet:
				  - Compute move and epsilon-closure for current DFA state
				  - If target set is new, create new DFA state and add to list/queue
				  - Add transition from current to target DFA state
		 3. Mark DFA states as final if any NFA state in their set is final
		 4. Return DFA with start state and all DFA states
		*/

		//Auxiliar set.
		Set<State> auxiliarInitial = new HashSet<>();
		auxiliarInitial.add(nfa.startState);


		//Auxiliar queue
		List<DfaState> marked = new LinkedList<>();
		Queue<DfaState> unmarked = new LinkedList<>();
		
		//step 1
		//creating DFA state.
		DfaState initial = new DfaState(epsilonClosure(auxiliarInitial));

		unmarked.add(initial);

		//step 2.
		while (!unmarked.isEmpty()){

			//adding new marked state
			marked.add(initial);

			//getting actual Dfastate
			DfaState current = unmarked.poll();

			//step 2.1
			for (Character character : alphabet){

				//step 2.2
				//I need to obtain the epsilon closure of move with character given by the alphabet.
				Set<State> move_set = epsilonClosure(move(current.nfaStates, character));
				
				//step 2.3
				//I need to check if it is a new state.
				DfaState checking_state=findDfaState(marked, move_set);
				if (checking_state==null){
					//if it is, create the new DfaState
					DfaState new_DfaState=new DfaState(move_set);
					
					//Adding the new transition from current state to newDfaState.
					current.addTransition(character, new_DfaState);

					//Adding the new state to unmarked.
					unmarked.add(new_DfaState);


				}else{
					//si ya existe un estado con el mismo set nfa, solo falta añadir la transicion.
					//if already exist a Dfastate with nfaState set then only add the transition.
					current.addTransition(character, checking_state);
				}
			}
		}

		//necesito crear el nuevo DFA.
		DFA new_Dfa = new DFA(initial, marked);
		return new_Dfa;
	}

	/**
	 * Computes the epsilon-closure of a set of NFA states.
	 * The epsilon-closure is the set of states reachable by epsilon (null) transitions.
	 *
	 * @param states The set of NFA states.
	 * @return The epsilon-closure of the input states.
	 */
	private static Set<State> epsilonClosure(Set<State> states) {
	/*
	 Pseudocode:
	 1. Initialize closure with input states
	 2. Use stack to process states
	 3. For each state, add all reachable states via epsilon transitions
	 4. Return closure set
	*/
		//I need a Auxiliar set for the return.
		Set<State> res = new HashSet<>();
		
		//I need a Auxiliar stack for processing states.
		Stack<State> stack = new Stack<>();
		
		//step 1.
		for (State state : states){
			res.add(state);
			//step 2.
			stack.push(state);
		}

		//step 3.
		while (!stack.empty()){

			State state = stack.pop();
	
			//Auxiliar variable, contains all states of epsilon transitions.
			List<State> list_epsilon= state.getEpsilonTransitions();
			
			//for every epsilon transition, add EndState´s transition to the stack.
			for (State auxState: list_epsilon){
				stack.push(auxState);
			}
			res.addAll(list_epsilon);
		}

		//step 4.
		return res;

	}

	/**
	 * Returns the set of states reachable from a set of NFA states by a given symbol.
	 *
	 * @param states The set of NFA states.
	 * @param symbol The input symbol.
	 * @return The set of reachable states.
	 */
	private static Set<State> move(Set<State> states, char symbol) {
		/*
		 Pseudocode:
		 1. For each state in input set:
			  - For each transition with given symbol:
				  - Add destination state to result set
		 2. Return result set
		*/

		//Auxiliar Set.
		Set<State> res = new HashSet<>();

		//step 1
		for (State state : states){
			for (State symbol_state : state.getTransitions(symbol)){
				res.add(symbol_state);				
			}
		}

		return res;
	}

	/**
	 * Finds an existing DFA state representing a given set of NFA states.
	 *
	 * @param dfaStates The list of DFA states.
	 * @param targetNfaStates The set of NFA states to search for.
	 * @return The matching DFA state, or null if not found.
	 */
	private static DfaState findDfaState(List<DfaState> dfaStates, Set<State> targetNfaStates) {
	   /*
	    Pseudocode:
	    1. For each DFA state in list:
		    - If its NFA state set equals target set, return DFA state
	    2. If not found, return null
	   */
	  	
	   //step 1
		for (DfaState state : dfaStates){
			//step 2
			if (state.getName().equals(targetNfaStates)) {
				return state;
			}
		}
		return null;
	}
}

//nos falta verificar si los estados finales se estan transmitiendo a los dfa.