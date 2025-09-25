package com.compiler.parser.syntax;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import com.compiler.parser.grammar.Grammar;
import com.compiler.parser.grammar.Production;
import com.compiler.parser.grammar.Symbol;
import com.compiler.parser.grammar.SymbolType;

/**
 * Calculates the FIRST and FOLLOW sets for a given grammar.
 * Main task of Practice 5.
 */
public class StaticAnalyzer {
    private final Grammar grammar;
    private final Map<Symbol, Set<Symbol>> firstSets;
    private final Map<Symbol, Set<Symbol>> followSets;

    //Agregamos los siguientes simbolos para la implementacion:
    private final Symbol epsilon = new Symbol("ε", SymbolType.TERMINAL);
    private final Symbol endInputSymbol = new Symbol("$", SymbolType.TERMINAL);

    public StaticAnalyzer(Grammar grammar) {
        this.grammar = grammar;
        this.firstSets = new HashMap<>();
        this.followSets = new HashMap<>();
    }

    /**
     * Calculates and returns the FIRST sets for all symbols.
     * @return A map from Symbol to its FIRST set.
     */
    public Map<Symbol, Set<Symbol>> getFirstSets() {
        /*
         * Pseudocode for FIRST set calculation:
         *
         * 1. For each symbol S in grammar:
         *      - If S is a terminal, FIRST(S) = {S}
         *      - If S is a non-terminal, FIRST(S) = {}
         *
         * 2. Repeat until no changes: x1: A x2: BaAaA | bBbBbB  
         *      For each production A -> X1 X2 ... Xn:
         *          - For each symbol Xi in the right-hand side:
         *              a. Add FIRST(Xi) - {ε} to FIRST(A)
         *              b. If ε is in FIRST(Xi), continue to next Xi
         *                 Otherwise, break
         *          - If ε is in FIRST(Xi) for all i, add ε to FIRST(A) // para este paso la estragtegia es un conteo de epsilon y de simbolos.
         *
         * 3. Return the map of FIRST sets for all symbols.
         */

        // Paso 1:
        //Para el caso de los terminales.
        for (Symbol simb:this.grammar.getTerminals()){
                HashSet<Symbol> set_aux =  new HashSet<>();
                set_aux.add(simb);
                firstSets.put(simb,set_aux);
        }

        //Conjunto con epsilon
        HashSet<Symbol> setAux= new HashSet<>();
        setAux.add(epsilon);
        firstSets.put(epsilon,setAux);

        //Para el caso de los no terminales.
        for (Symbol simb:this.grammar.getNonTerminals()){
            HashSet<Symbol> set_aux =  new HashSet<>();
            firstSets.put(simb,set_aux);
        }

        //Paso 2.
        boolean changes;
        do {
            changes=false;
            //Por cada produccion
            for (Production prod: grammar.getProductions()){

                //Contador de simbolos auxiliar para el paso b
                int countSymbol=prod.getRight().size();
                //Contador de epsilons auxiliar para el paso b.
                int countEpsilon=0;

                //Por cada simbolo del lado derecho
                for (Symbol simb : prod.getRight()){

                    Set<Symbol> temp = new HashSet<>(firstSets.get(simb));
                    boolean flag = firstSets.get(simb).contains(epsilon);
                    temp.remove(epsilon);

                    //Obtenemos el FIRST del lado izquierdo
                    Set<Symbol> firstLeft = firstSets.get(prod.getLeft());

                    //Agregamos el conjunto temp al FIRST de A
                    if (firstLeft.addAll(temp)) {
                        //si hubo cambios, actualizamos la variable del ciclo.
                        changes = true;
                    }

                    //b. If ε is in FIRST(Xi), continue to next Xi
                    //Otherwise, break
                    if (!flag){
                        break;
                    }else{
                        countEpsilon++;
                    }

                    //If ε is in FIRST(Xi) for all i, add ε to FIRST(A)
                    if (countEpsilon==countSymbol){
                        if (firstLeft.add(epsilon)) {
                            changes = true;
                        }
                    }
                }
            }
        }while(changes);

        return firstSets;
    }

    /**
     * Calculates and returns the FOLLOW sets for non-terminals.
     * @return A map from Symbol to its FOLLOW set.
     */
    public Map<Symbol, Set<Symbol>> getFollowSets() {
        /*
         * Pseudocode for FOLLOW set calculation:
         *
         * 1. For each non-terminal A, FOLLOW(A) = {}
         * 2. Add $ (end of input) to FOLLOW(S), where S is the start symbol
         *
         * 3. Repeat until no changes:
         *      For each production B -> X1 X2 ... Xn:
         *          For each Xi (where Xi is a non-terminal):
         *              a. For each symbol Xj after Xi (i < j <= n):
         *                  - Add FIRST(Xj) - {ε} to FOLLOW(Xi)
         *                  - If ε is in FIRST(Xj), continue to next Xj
         *                    Otherwise, break
         *              b. If ε is in FIRST(Xj) for all j > i, add FOLLOW(B) to FOLLOW(Xi)
         *
         * 4. Return the map of FOLLOW sets for all non-terminals.
         *
         * Note: This method should call getFirstSets() first to obtain FIRST sets.
         */
        
         //Obtener FirstsSets
         this.getFirstSets();

        //Paso 1:
            for (Symbol simb : grammar.getNonTerminals()){
                //declaramos un nuevo set vacio.
                HashSet<Symbol> set_aux =  new HashSet<>();
                this.followSets.put(simb, set_aux);
            }

        //Paso 2:
            //Add $ (end of input) to FOLLOW(S), where S is the start symbol
            this.followSets.get(this.grammar.getStartSymbol()).add(endInputSymbol);
        

        //Paso 3:
            boolean changes;
            //Mientras no haya cambios repite:
            do {
                changes=false;
                
                //Para cada produccion B -> X1 X2 ... Xn:
                for (int i=0; i<this.grammar.getProductions().size();i++){ 
                    
                    //Obtenemos la produccion actual.
                    Production prod = this.grammar.getProductions().get(i);
                    //Para cada xi:
                    for (Symbol xi : prod.getRight()){

                        //a)
                        //Si xi es un simbolo no terminal.
                        if (this.grammar.getNonTerminals().contains(xi)){
                            //var auxiliar para saber si todos los xj continenen epsilon.
                            boolean epsilonInAll = true;

                            //para cada simbolo Xj despues de Xi tq se cumple que (i<j<=n), n= cant de producciones.                            
                            for (int j = i+1; j < prod.getRight().size(); j++){
                                
                                //primero necesito encontrar al simbolo de Xj
                                Symbol xj= prod.getRight().get(j);

                                //ahora necesito encontrar el conjunto first de xj y obtener una copia
                                Set<Symbol> xjFIRST = new HashSet<>(this.getFirstSets().get(xj));
                                
                                //eliminamos el epsilon y guardamos la bandera.
                                boolean hasEpsilon =xjFIRST.remove(epsilon);

                                //Agregamos FIRST(Xj) - {ε} a FOLLOW(Xi) y verificamos si hubo cambios
                                changes=this.followSets.get(xi).addAll(xjFIRST);
                                
                                //If ε is in FIRST(Xj), continue to next Xj
                                //Otherwise, break
                                if (!hasEpsilon){
                                    break;
                                }
                            }

                            //b) If ε is in FIRST(Xj) for all j > i, add FOLLOW(B) to FOLLOW(Xi)
                            if (i == prod.getRight().size() - 1 || epsilonInAll) {
                                changes=this.followSets.get(xi).addAll(this.followSets.get(prod.getLeft()));
                                
                            }

                        }
                        
                    }
                    
                }  
            
            } while (changes);

        return this.followSets;

    }
}