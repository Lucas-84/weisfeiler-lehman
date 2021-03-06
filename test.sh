#!/bin/bash

pad=$(printf '%0.1s' "."{1..40})
good=0
bad=0

for f in tests/*.in; do
  # Calcul de l'algo 
  res=$(./weisfeiler-lehman $f | head -c 3)

  # Résultat attendu
  if [ $(echo $f | grep "\-perm.in") ]; then
    expected=oui
  else
    expected=non
  fi

  # Affichage du resultat et comptage du score
  printf '%s' "$f"
  printf '%*.*s' 0 $((40 - ${#f})) "$pad"
  if [ $res = $expected ]; then
    printf '%s\n' "OK"
    good=$((good + 1))
  else
    printf '%s\n' "FAIL"
    bad=$((bad + 1))
  fi
done

# Score final
printf 'Success/fail: %d/%d\n' $good $bad
