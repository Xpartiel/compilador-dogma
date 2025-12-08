/* codegen.h
 *
 * Generador de código intermedio para la Máquina Virtual FIS-25
 *
 * Interfaz pública para generar código FIS-25 a partir del AST.
 * Diseñado para integrarse con ast.h / ast.c y el resto del compilador.
 *
 * Uso típico:
 *   FILE *out = fopen("out.fis","w");
 *   codegen_init(out);
 *   codegen_gen_sequence(root);   // root es AST_SEQUENCE (raíz)
 *   codegen_finish();
 *   fclose(out);
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"

/* Inicializa el generador y le pasa el FILE* donde escribir las instrucciones.
 * Debe llamarse antes de generar cualquier instrucción. */
void codegen_init(FILE *out);

/* Finaliza la generación, libera recursos internos y añade cualquier cierre
 * necesario (comentarios, HALT opcional). Después de llamar, no usar más emit. */
void codegen_finish(void);

/* Generación de código:
 * - codegen_gen_sequence: genera todo el código para un nodo SEQUENCE (bloque/raíz).
 * - codegen_gen_statement: genera código para una sola sentencia.
 * - codegen_gen_expr: genera código para una expresión y devuelve malloc'd string
 *   con el nombre (variable literal o temporal) que contiene el resultado.
 *   El llamador debe free() la cadena retornada cuando corresponda.
 */
bool codegen_gen_statement(ASTNode *stmt);
bool codegen_gen_sequence(ASTNode *seq);
char *codegen_gen_expr(ASTNode *expr);

/* utilidades (opcionales) */
char *codegen_new_temp(void);   /* devuelve malloc'd name "Tn" */
char *codegen_new_label(void);  /* devuelve malloc'd name "Ln" */

#endif /* CODEGEN_H */
