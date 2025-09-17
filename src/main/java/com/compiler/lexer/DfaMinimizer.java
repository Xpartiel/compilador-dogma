
/**
 * DfaMinimizer
 * -------------
 * This class provides an implementation of DFA minimization using the table-filling algorithm.
 * It identifies and merges equivalent states in a deterministic finite automaton (DFA),
 * resulting in a minimized DFA with the smallest number of states that recognizes the same language.
 *
 * Main steps:
 *   1. Initialization: Mark pairs of states as distinguishable if one is final and the other is not.
 *   2. Iterative marking: Mark pairs as distinguishable if their transitions lead to distinguishable states,
 *      or if only one state has a transition for a given symbol.
 *   3. Partitioning: Group equivalent states and build the minimized DFA.
 *
 * Helper methods are provided for partitioning, union-find operations, and pair representation.
 */
package com.compiler.lexer;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import com.compiler.lexer.dfa.DFA;
import com.compiler.lexer.dfa.DfaState;
import com.compiler.lexer.nfa.State;


/**
 * Implements DFA minimization using the table-filling algorithm.
 */
/**
 * Utility class for minimizing DFAs using the table-filling algorithm.
 */
public class DfaMinimizer {
    /**
     * Default constructor for DfaMinimizer.
     */
        public DfaMinimizer() {
            // TODO: Implement constructor if needed
        }

    /**
     * Minimizes a given DFA using the table-filling algorithm.
     *
     * @param originalDfa The original DFA to be minimized.
     * @param alphabet The set of input symbols.
     * @return A minimized DFA equivalent to the original.
     */
    public static DFA minimizeDfa(DFA originalDfa, Set<Character> alphabet) {
    // TODO: Implement minimizeDfa
    /*
     Pseudocode:
     1. Collect and sort all DFA states
     2. Initialize table of state pairs; mark pairs as distinguishable if one is final and the other is not
     3. Iteratively mark pairs as distinguishable if their transitions lead to distinguishable states or only one has a transition
     4. Partition states into equivalence classes (using union-find)
     5. Create new minimized states for each partition
     6. Reconstruct transitions for minimized states
     7. Set start state and return minimized DFA
    */
        // Paso 1: Obtener y ordenar todos los estados
        List<DfaState> allStates = new ArrayList<>(originalDfa.allStates);
        allStates.sort(Comparator.comparingInt(s -> s.id));

        // Paso 2: Inicializar tabla de pares (Pair -> Boolean) con los distinguibles iniciales
        Map<Pair, Boolean> table = new HashMap<>();

        for (int i = 0; i < allStates.size(); i++) {
            for (int j = i + 1; j < allStates.size(); j++) {
                DfaState s1 = allStates.get(i);
                DfaState s2 = allStates.get(j);
                Pair pair = new Pair(s1, s2);
                boolean mark = s1.isFinal() != s2.isFinal();
                table.put(pair, mark);
            }
        }

        // Paso 3: Iterar y marcar pares distinguibles por transiciones
        boolean changed;
        do {
            changed = false;
            for (int i = 0; i < allStates.size(); i++) {
                for (int j = i + 1; j < allStates.size(); j++) {
                    DfaState s1 = allStates.get(i);
                    DfaState s2 = allStates.get(j);
                    Pair pair = new Pair(s1, s2);

                    if (table.get(pair)) continue; // Ya marcado como distinguible

                    for (Character symbol : alphabet) {
                        DfaState t1 = s1.getTransition(symbol);
                        DfaState t2 = s2.getTransition(symbol);

                        if (t1 == null && t2 == null) continue;

                        if (t1 == null || t2 == null) {
                            table.put(pair, true);
                            changed = true;
                            break;
                        }

                        Pair targetPair = new Pair(t1, t2);
                        Boolean targetMarked = table.get(targetPair);

                        if (targetMarked != null && targetMarked) {
                            table.put(pair, true);
                            changed = true;
                            break;
                        }
                    }
                }
            }
        } while (changed);

        // Paso 4: Crear particiones (grupos de estados equivalentes)
        List<Set<DfaState>> partitions = createPartitions(allStates, table);

        // Paso 5: Crear nuevos estados para cada partición
        Map<DfaState, DfaState> oldToNew = new HashMap<>();
        Set<DfaState> newStates = new HashSet<>();

        for (Set<DfaState> group : partitions) {
            // Elegimos el primero como representativo
            //DfaState representative = group.iterator().next();

            // Creamos el nuevo estado basado en los NFA states del grupo
            Set<State> combinedNfaStates = new HashSet<>();
            for (DfaState old : group) {
                combinedNfaStates.addAll(old.getName());
            }
            DfaState newState = new DfaState(combinedNfaStates);

            // La finalidad será verdadera si alguno en el grupo es final
            boolean isFinal = group.stream().anyMatch(DfaState::isFinal);
            newState.setFinal(isFinal);

            // Asociamos todos los antiguos con el nuevo
            for (DfaState old : group) {
                oldToNew.put(old, newState);
            }

            newStates.add(newState);
        }

        // Paso 6: Construir transiciones para los nuevos estados
        for (DfaState oldState : allStates) {
            DfaState newSource = oldToNew.get(oldState);
            if (newSource == null) continue;

            for (Map.Entry<Character, DfaState> entry : oldState.getTransitions().entrySet()) {
                Character symbol = entry.getKey();
                DfaState oldTarget = entry.getValue();
                DfaState newTarget = oldToNew.get(oldTarget);

                if (newTarget != null) {
                    newSource.addTransition(symbol, newTarget);
                }
            }
        }

        // Paso 7: Identificar nuevo estado inicial
        DfaState newStart = oldToNew.get(originalDfa.startState);

        // Retornar nuevo DFA
        return new DFA(newStart, new ArrayList<>(newStates));
        
    }

    /**
     * Groups equivalent states into partitions using union-find.
     *
     * PSEUDOCODE
     * 
     * 1. Initialize each state as its own parent
     * 2. For each pair not marked as distinguishable, union the states
     * 3. Group states by their root parent
     * 4. Return list of partitions
     * 
     * @param allStates List of all DFA states.
     * @param table Table indicating which pairs are distinguishable.
     * @return List of partitions, each containing equivalent states.
     */
    private static List<Set<DfaState>> createPartitions(List<DfaState> allStates, Map<Pair, Boolean> table) {
        List<Set<DfaState>> res = new LinkedList<>();
        Map<DfaState,DfaState> parent = new HashMap<>();

        /* Step 1 */
        for (DfaState state : allStates ) {
            parent.put(state, state);
        }

        /* Step 2 */
        for ( Pair par : table.keySet() ) {
            if( !table.get(par) ){
                union(parent, par.s1, par.s2);
            }
        }

        /* Step 3 */
        DfaState tempRoot;
        HashSet<DfaState> tempTree;
        boolean foundTree;

        // For each state
        for( DfaState state1 : allStates ) {
            // Consider its root
            tempRoot = find(parent, state1);
            // Asume its set/tree doesn't exist
            foundTree = false;
            // For each set/tree in the known forest...
            for( Set<DfaState> tree : res ){
                // If the representative root is found
                if( tree.contains(tempRoot) ){
                    // Add current state
                    tree.add(state1);
                    // Indicate its set/tree has been found
                    foundTree = true;
                    break;
                }
            }
            // No set/tree was found with the expected root
            if( !foundTree ){
                // It shall be created
                tempTree = new HashSet<>();
                // Its first element shall be its root
                tempTree.add( tempRoot );
                // Insert current state
                tempTree.add( state1 );
                // The set/tree shall be included into the forest
                res.add( tempTree );
            }
        }

        return res;
    }

    /**
     * Finds the root parent of a state in the union-find structure.
     * Implements path compression for efficiency.
     *
     * @param parent Parent map.
     * @param state State to find.
     * @return Root parent of the state.
     */
    private static DfaState find(Map<DfaState, DfaState> parent, DfaState state) {
    /*
     Pseudocode:
     If parent[state] == state, return state
     Else, recursively find parent and apply path compression
     Return parent[state]
    */
        if (parent.get(state) == state){
            return state; //we have find the root 
        }
        DfaState root = find(parent,parent.get(state));
        parent.put(state, root);
        return root;

        //     if (parent.get(state) != state) {
        //         parent.put(state, find(parent, parent.get(state))); // path compression
        //     }
        //     return parent.get(state);
            
    }

    /**
     * Unites two states in the union-find structure.
     *
     * @param parent Parent map.
     * @param s1 First state.
     * @param s2 Second state.
     */
    private static void union(Map<DfaState, DfaState> parent, DfaState s1, DfaState s2) {
    /*
     Pseudocode:
     Find roots of s1 and s2
     If roots are different, set parent of one to the other
    */

        DfaState roots1= find(parent, s1);
        DfaState roots2= find(parent, s2);
        if (!roots1.equals(roots2)){
            parent.put(roots2, roots1);
        }
    }

    /**
     * Helper class to represent a pair of DFA states in canonical order.
     * Used for table indexing and comparison.
     */
    private static class Pair {
        final DfaState s1;
        final DfaState s2;

        /**
         * Constructs a pair in canonical order (lowest id first).
         * @param s1 First state.
         * @param s2 Second state.
         */
        public Pair(DfaState s1, DfaState s2) {
            /*
             Pseudocode:
             Assign s1 and s2 so that s1.id <= s2.id
            */
            if(s1.id <=s2.id){
                this.s1=s1;
                this.s2=s2;
            }else{
                this.s1=s2;
                this.s2=s1;
            }
        }

        @Override
        public boolean equals(Object o) {
            /*
             Pseudocode:
             Return true if both s1 and s2 ids match
            */
        
            //case is instance of State
                if (this == o) return true;
                if (!(o instanceof Pair)) return false;
                Pair other = (Pair) o;
            return this.s1.id == other.s1.id && this.s2.id == other.s2.id;
        }

        @Override
        public int hashCode() {
            /*
             Pseudocode:
             Return hash of s1.id and s2.id
             */
            return Objects.hash(s1.id, s2.id);
        }
    }
}
