# VITERBI

import numpy as np
states = ['H','L']
initial_prob = {'H' : -1,'L' : -1}
transitions_prob = {'H':{'H':-1,'L':-1},'L' : {'H':-1.322,'L':-0.737}}
emission_prob = {'H' : {'A':-2.322,'G':-1.737,'C':-1.737,'T':-2.322},
                 'L' : {'A':-1.737,'G':-2.322,'C':-2.322,'T':-1.737}}


sequence = 'GGCACTGAA'

#Stroring the values for each recursive call (memoization)
probability_table = np.zeros([len(sequence),len(states)])

def viterbi(state,index,position) : 
    #base condition
    if(index==0) : 
        #print(initial_prob[state] + emission_prob[state][sequence[index]])
        probability_table[index][states.index(state)] = (initial_prob[state] + emission_prob[state][sequence[index]])
        return initial_prob[state] + emission_prob[state][sequence[index]]
    else : 
        #If this has already been calculated and stored then no need to calculate again
        x=probability_table[index][states.index(state)]
        if(x!=0):
            return x
        #else call the function recursively for decremented value of index
        emission = emission_prob[state][sequence[index]] 
        values = []
        for j in states : 
            p = transitions_prob[j][state] + viterbi(j,index-1,position-1)
            values.append(p)
        #print(emission + max(values))
        probability_table[index][states.index(state)] = emission + max(values)
        return emission + max(values)

#calling the function for each ending state along with the index of last element
for i in states : 
    viterbi(i,len(sequence)-1,len(sequence))

path=[]
probability_table = probability_table.tolist()
#finding the path (max)
for i in probability_table : 
    path.append(states[i.index(max(i))])    

print("probability table : ")
for i in probability_table : 
    print(i)

print("Path : ",path)

#FORWARD

import numpy as np
states = ['H','L']
initial_prob = {'H' : 0.5,'L' : 0.5}
transitions_prob = {'H':{'H':0.5,'L':0.5},'L' : {'H':0.4,'L':0.6}}
emission_prob = {'H' : {'A':0.2,'G':0.3,'C':0.3,'T':0.2},
                 'L' : {'A':0.3,'G':0.2,'C':0.2,'T':0.3}}

sequence="GGCA"

probability_table = np.zeros([len(states),len(sequence)])


for i in range(len(sequence)) : 
    element = sequence[i]
    if(i==0):
        for j in range(len(states)) :
            probability_table[j][i] = initial_prob[states[j]] * emission_prob[states[j]][element]
    else : 
        for k in range(len(states)) : 
            p1=0
            for j in range(len(states)) : 
                p = probability_table[j][i-1]
                q = transitions_prob[states[j]][states[k]] * emission_prob[states[k]][element]
                p1+=p*q
            probability_table[k][i] = p1

print("Probability table : ",probability_table,sep='\n')

expected =0 
for i in range(len(states)) : 
    expected += probability_table[i][-1]
print("Required probability for the given sequence  : ",expected)

#MDP

# rewards = {'Sun': 4, 'Wind': 0, 'Hail': -8}
# transitions = {'Sun': {1: [('Sun', 0.5), ('Wind', 0.5)]},
#               'Wind': {1: [('Sun', 0.5), ('Hail', 0.5)]},
#               'Hail': {1: [('Wind', 0.5), ('Hail', 0.5)]}}
 
rewards = {'PU': 0, 'PF': 0, 'RU': 10, 'RF': 10}
transitions = {'PU': {'S': [('PU', 1)],
                      'A': [('PU', 0.5), ('PF', 0.5)]},
                'PF': {'S': [('PU', 0.5), ('RF', 0.5)],
                      'A': [('PF', 1)]},
                'RU': {'S': [('PU', 0.5), ('RU', 0.5)],
                      'A': [('PU', 0.5), ('PF', 0.5)]},
                'RF': {'S': [('RU', 0.5), ('RF', 0.5)],
                      'A': [('PF', 1)]}}
 
d1, d2 = dict(rewards), dict(rewards)
discountFactor = 0.9
tolerance = 1e-3
 
count = 0
while True:
    for i in transitions:
        m = -float('inf')
        for j in transitions[i]:
            m = max(m, sum([k[1] * d2[k[0]] for k in transitions[i][j]]))
        d1[i] = rewards[i] + discountFactor * m
    delta = max([abs(d1[k] - d2[k]) for k in d1])
    if delta < tolerance: break
    d2 = dict(d1)
    count += 1
print(d1, count)

