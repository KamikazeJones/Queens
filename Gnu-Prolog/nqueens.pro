/* Dieses Prädikat erzeugt eine Liste, die eine einzige Lösung darstellt.
    Es ist sichergestellt, dass jeder Wert zwischen 1 und N genau einmal auftritt. */

 n_damen(N,Ls) :- length(Ls,N),
                fd_domain(Ls,1,N),
                fd_all_different(Ls),
                constraint_damen(Ls),
                fd_labeling(Ls,[variable_method(random)]).

 /* Dieses Prädikat stellt sicher,
    dass alle Stellungen die Lösungsbedingungen erfuellen */

 constraint_damen([]).
 constraint_damen([X|Xs]) :- nicht_schlagen(X,Xs,1), constraint_damen(Xs).

 /* Mit diesem Prädikat wird sichergestellt,
    dass zwei Damen nicht auf einer Diagonalen stehen */

 nicht_schlagen(_,[],_).
 nicht_schlagen(X,[Y|Xs],N) :- X#\=Y+N, X#\=Y-N, T#=N+1, nicht_schlagen(X,Xs,T).