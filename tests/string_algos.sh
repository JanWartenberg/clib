LOG="string_algos_10x.log"

rm -f "$LOG"

for i in {1..10}; do
  echo "=== RUN $i ===" >> "$LOG"
  ./string_algos_tests >> "$LOG" 2>&1
done

echo "DONE" >> "$LOG"
echo "Summary:"
grep -c "ALL TESTS PASSED" "$LOG"
