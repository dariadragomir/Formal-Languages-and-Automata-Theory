# '.' is the lambda simbol 
def initialize_dpda(states, alphabet, stack_alphabet, transition_table, start_state, start_stack_symbol, accept_states):
    dpda = {}
    dpda['states'] = states
    dpda['alphabet'] = alphabet
    dpda['stack_alphabet'] = stack_alphabet
    dpda['transition_table'] = transition_table
    dpda['current_state'] = start_state
    dpda['stack'] = [start_stack_symbol]
    dpda['accept_states'] = accept_states
    return dpda

def transition(dpda, input_symbol):
    current_state = dpda['current_state']
    stack = dpda['stack']
    transition_table = dpda['transition_table']
    if not stack:
        return False  
    stack_top = stack[-1]
    if (current_state, input_symbol, stack_top) not in transition_table:
        return False
    else:
        new_state, new_stack_top, stack_action = transition_table[(current_state, input_symbol, stack_top)]
        dpda['current_state'] = new_state
        dpda['stack'].pop()
        if stack_action != '.':
            for symbol in reversed(stack_action):
                dpda['stack'].append(symbol)
        return True

def is_accepted(dpda):
    return dpda['current_state'] in dpda['accept_states'] and len(dpda['stack']) == 1 and dpda['stack'][0] == '.'

def check_acceptance(word, dpda):
    for symbol in word:
        if symbol not in dpda['alphabet']:
            return "not accepted"
        if not transition(dpda, symbol):
            return "not accepted"
    if is_accepted(dpda):
        return "accepted"  
    else:
        return "not accepted"

states = {'q0', 'q1', 'q2'}
alphabet = {'a', 'b'}
stack_alphabet = {'0', '1'}
transition_table = {
    ('q0', 'a', '0'): ('q1', '1', '.'),
    ('q0', 'b', '1'): ('q2', '0', '.'),
    ('q1', 'a', '1'): ('q1', '1', '.'),
    ('q1', 'b', '1'): ('q1', '1', '.'),
    ('q2', 'a', '0'): ('q2', '0', '.'),
    ('q2', 'b', '0'): ('q2', '0', '.'),
}
start_state = 'q0'
start_stack_symbol = '0'
accept_states = {'q2'}

dpda = initialize_dpda(states, alphabet, stack_alphabet, transition_table, start_state, start_stack_symbol, accept_states)

word = "ababab"
print(check_acceptance(word, dpda))
