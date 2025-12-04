/* Marquee simple
Supone las funciones runtime: read_line, clear_matrix, set_pixel, refresh_display, read_keys, strlen, substr, atoi */

/* atoi -> ASCII to Integer */

/* --- Variables globales */
# offset <- 0;
# prev_keyA <- 0;
# prev_keyB <- 0;

/* --- Lee mensaje desde stdin (string) */
@ message <- read_line(); 
(message = "") ? { 
    /* si no ingresa nada, ponemos mensaje por defecto */
    message <- "0123456789.$";
}

/* --------------------------------------------------
 draw_char: dibuja un caracter permitido en (base_x, base_y)
 Implementacion simple: para cada caracter soportado (0-9, '.', '$')
 colocamos set_pixel relativos manualmente (5 filas x 3 cols)
 -------------------------------------------------- */
# -> draw_char( @ ch , # base_x , # base_y ) {
    /* fila 0..4, col 0..2 designadas manualmente para cada char
    Nótese que usamos substr(ch,0,1) para comparar char */
    ( ch = "0" ) ? {
        /* 0: 111 / 101 / 101 / 101 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1);                    set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2);                    set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 0, base_y + 3);                    set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "1" ) ? {
        /* 010 / 110 / 010 / 010 / 111 */
        set_pixel(base_x + 1, base_y + 0);
        set_pixel(base_x + 0, base_y + 1); set_pixel(base_x + 1, base_y + 1);
        set_pixel(base_x + 1, base_y + 2);
        set_pixel(base_x + 1, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "2" ) ? {
        /* 111 / 001 / 111 / 100 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 0, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "3" ) ? {
        /* 111 / 001 / 111 / 001 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "4" ) ? {
        /* 101 / 101 / 111 / 001 / 001 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1); set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "5" ) ? {
        /* 111 / 100 / 111 / 001 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "6" ) ? {
        /* 111 / 100 / 111 / 101 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 0, base_y + 3); set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "7" ) ? {
        /* 111 / 001 / 010 / 100 / 100 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 1, base_y + 2);
        set_pixel(base_x + 0, base_y + 3);
        set_pixel(base_x + 0, base_y + 4);
        :: 0;
    } :|
    ( ch = "8" ) ? {
        /* 111 / 101 / 111 / 101 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1); set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 0, base_y + 3); set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "9" ) ? {
        /* 111 / 101 / 111 / 001 / 111 */
        set_pixel(base_x + 0, base_y + 0); set_pixel(base_x + 1, base_y + 0); set_pixel(base_x + 2, base_y + 0);
        set_pixel(base_x + 0, base_y + 1); set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 0, base_y + 2); set_pixel(base_x + 1, base_y + 2); set_pixel(base_x + 2, base_y + 2);
        set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 0, base_y + 4); set_pixel(base_x + 1, base_y + 4); set_pixel(base_x + 2, base_y + 4);
        :: 0;
    } :|
    ( ch = "." ) ? {
        /* punto en fila 4, centro */
        set_pixel(base_x + 1, base_y + 4);
        :: 0;
    } :|
    ( ch = "$" ) ? {
        /* simbolo $ -- simple cross-like */
        set_pixel(base_x + 1, base_y + 0);
        set_pixel(base_x + 0, base_y + 1); set_pixel(base_x + 1, base_y + 1); set_pixel(base_x + 2, base_y + 1);
        set_pixel(base_x + 1, base_y + 2);
        set_pixel(base_x + 0, base_y + 3); set_pixel(base_x + 1, base_y + 3); set_pixel(base_x + 2, base_y + 3);
        set_pixel(base_x + 1, base_y + 4);
        :: 0;
    } :
    {
        /* caracter no soportado -> nada */
        :: 0;
    }
};

/* --------------------------------------------------
 draw_message: dibuja message con offset horizontal
 cada caracter ocupa 3 cols + 1 espacio = 4
 -------------------------------------------------- */
# -> draw_message( @ msg , # offset_local ) {
    # i <- 0;
    # len <- strlen(msg);
    # x <- offset_local;
    << /* bucle while simulacro (infinito con break) */
        ( i >= len ) ? { !!; };
        @ ch <- substr(msg, i, 1);
        draw_char(ch, x, 0);   /* alineado en y=0 */
        x <- x + 4;
        i <- i + 1;
    >>
    :: 0;
};

/* --------------------------------------------------
 Bucle principal: limpia, dibuja, refresh, lee teclas y detecta falling edge
 Teclas: keyA (mover izquierda), keyB (mover derecha)
 read_keys() devuelve "a,b" (ej: "1,0")
 -------------------------------------------------- */
<<   /* BEGIN_LOOP*/
    /* limpiar matriz */
    clear_matrix();

    /* dibujar mensaje */
    draw_message(message, offset);

    /* refrescar display */
    refresh_display();

    /* leer teclas */
    @ kstr <- read_keys();           /* ej "1,0" */
    @ a_str <- substr(kstr, 0, 1);   /* primer char */
    @ b_str <- substr(kstr, 2, 1);   /* tercer char (asumimos "donde , esta en pos 1") */

    # keyA <- atoi(a_str);
    # keyB <- atoi(b_str);

    /* detectar falling edge: prev = 1 && current = 0 */
    ( prev_keyA = 1 && keyA = 0 ) ? {
        offset <- offset - 1;
    } ;
    ( prev_keyB = 1 && keyB = 0 ) ? {
        offset <- offset + 1;
    } ;

    /* actualizar prev */
    prev_keyA <- keyA;
    prev_keyB <- keyB;


>> ; /* END_LOOP SEQUENCE_SEPARATOR */


