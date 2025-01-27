(ns queens.core
  (:require [clojure.core.logic :refer :all]
            [clojure.core.logic.fd :as fd])
            ; [clojure.tools.trace :refer [trace]]
            
  (:gen-class))

; core.logic
; variable Anzahl an logischen Variablen

;    1 2 3 4
;  1 . . x .
;  2 x . . .      
;  3 . . . x
;  4 . x . .

(defn generate-vars [n]
  (repeatedly n lvar))

;      (all
;       (everyg (fn [idx]
;                 (fd/+ (nth vars idx) (inc idx) (nth diag-up idx)))
;               (range n))
;       (fd/distinct diag-up)
;       (everyg (fn [idx]
;                 (fresh [tmp]
;                        (fd/+ (nth vars idx) n tmp)
;                        (fd/- tmp (inc idx) (nth diag-down idx))))
;               (range n))
;       (fd/distinct diag-down))))

;; (defn constraint-diagonal [vars]
;;   (let [n (count vars)]
;;     (all
;;      (everyg (fn [c]) 
;;         (fresh [s d]
;;                (fd/+ (nth vars c) (inc c) s)
;;                (fd/- (nth vars c) (- (inc c) n) d)
;;                (everyg (fn [idx]
;;                          (fresh [tmp]
;;                                 (fd/+ (nth vars idx) (inc idx) tmp)
;;                                 (fd/!= s tmp)))
;;                        (range (inc c) n))
;;                (everyg (fn [idx]
;;                          (fresh [tmp]
;;                                 (fd/- (nth vars idx) (- (inc idx) n) tmp)
;;                                 (fd/!= d tmp)))
;;                        (range (inc c) n)))
;;         (range n)))))

(defn fdf_a_b!=c [fdf a b c] 
  (fresh [t]
         (fdf a b t)
         (fd/!= t c)))

(defn can-not-take [x vs n]
  (let [[y & xs] vs] 
    (if (empty? vs)
      succeed
      (conde [(fdf_a_b!=c fd/+ y n x)
              (fdf_a_b!=c fd/+ x n y)
              (can-not-take x xs (inc n))]))))

(defn constraint-queens [vars]
  (let [[v & vs] vars] 
     (if (empty? vs)
       succeed
       (conde [(can-not-take v vs 1)
               (constraint-queens vs)]))))

(defn queens [n]
  (let [vars (generate-vars n)]
    (run 2 [q]
          (everyg #(fd/in % (fd/interval 1 n)) vars)
          (fd/distinct vars)
          (constraint-queens vars)
          (== q vars))))

(defn -main
  "n queens problem"
  [& args]
  (println "n Damen platzieren, so dass sie sich nicht gegenseitig schlagen können")
  (time(println (queens 19))))
