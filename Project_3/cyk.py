def cyk(input_string, grammar, nonterminals):
    length = len(input_string)
    num_nonterminals = len(nonterminals)
    
    parse_table = []
    for i in range(length):
        parse_table.append([])
        for i in range(length):
            parse_table[-1].append([False for j in range(num_nonterminals)])
    
    backpointer_table = []
    for i in range(length):
        backpointer_table.append([[] for j in range(length)])

    nonterminal_index = {}
    for i, nonterminal in enumerate(nonterminals):
        nonterminal_index[nonterminal] = i

    for pos in range(length):
        for production in grammar:
            left_side, right_side = production
            if len(right_side) == 1 and right_side[0] == input_string[pos]:
                nonterminal_idx = nonterminal_index[left_side]
                parse_table[0][pos][nonterminal_idx] = True
    
    for span_length in range(2, length+1):
        for span_start in range(length-span_length+1):
            for partition in range(1, span_length):
                for production in grammar:
                    left_side, right_side = production
                    if len(right_side) == 2:
                        left_nonterminal, right_nonterminal = right_side
                        if left_nonterminal in nonterminal_index and right_nonterminal in nonterminal_index:
                            left_idx = nonterminal_index[left_nonterminal]
                            right_idx = nonterminal_index[right_nonterminal]
                            current_idx = nonterminal_index[left_side]
                            if parse_table[partition-1][span_start][left_idx] and parse_table[span_length-partition-1][span_start+partition][right_idx]:
                                parse_table[span_length-1][span_start][current_idx] = True
                                backpointer_table[span_start][span_length-1].append((partition, left_idx, right_idx))
    
    start_symbol_index = nonterminal_index[nonterminals[0]]
    if parse_table[length-1][0][start_symbol_index]:
        return "accepted"
    else:
        return "not accepted"

nonterminals = ["S", "A", "B"]
grammar = [
    ("S", ["A", "B"]),
    ("S", ["B", "A"]),
    ("A", ["a"]),
    ("B", ["b"])
]

word = "ab"
result = cyk(word, grammar, nonterminals)
print(result)
