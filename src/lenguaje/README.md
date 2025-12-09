# Lenguaje de Programación y Compilador
## Symi (sm)

# Instrucciones de uso
1. Posicionese en la carpeta actual (src/lenguajes)
2. Abra una terminal
3. Ejecute el comando
> bison -d parser.y
4. Ejecute el comando
> flex lexer.l
5. Compile los archivos __C__ mediante el comando
> gcc -o parserito lex.yy.c parser.tab.c  ast.c type.c semantic.c codegen.c -lfl
6. Compile su codigo fuente __archivo.sm__ con el comando
> ./parserito < archivo.sm
7. El codigo resultante se escribirá en el archivo __out.fis__
8. Copiarlo dentro del simulador de la maquina virtual [__FIS-25__](https://amm-gdev.itch.io/fis-25)