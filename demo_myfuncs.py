


### Python Lab Assignments

# computing factorial

def factorial(n):
    s = 1
    for k in range(1,n):
        s = s*(k+1)
    return s

# exponetial function


def exponential(x,n):
    e = 2.7182818284590451
    x0 = int(round(x))
    z = x-x0
    if (n<=0):
        return 1.0
    sum=0
    for k in range (n):
        sum = sum + (z**k)/factorial(k)

    r = (e**x0)*sum
    return r


#nat log func using newtons method with inital guess s=x

def nat_log(x,n, exp_terms):
    #iniit guess
    s=x
    for k in range(n):
        exp = exponential(-s, exp_terms)
        s = s-1+x*exp

    return s
        
    return 0

    



if __name__ == '__main__':
# test factorial
    test_f = 5
    fact_res = factorial(test_f)
    print(f"--- factorial test ---")
    print(f"input: {test_f}")
    print(f"result: {fact_res}")
    print(f"expected: 120") #5! is 120

# test exp
    test_x = 2.0
    exp_res = exponential(test_x, 15)
    print(f"--- exp test ---")
    print(f"input: {test_x}")
    print(f"result: {exp_res}")
    print(f"expected (approx): 7.389056") # approx answer
    
    print("-" * 20)

# test nat log
    test_ln_x = 7.389056
    # We use 20 iterations for Newton and 15 terms for the internal exp
    ln_res = nat_log(test_ln_x, 20, 15)
    print(f"--- nat log test ---")
    print(f"input: {test_ln_x}")
    print(f"result: {ln_res}")
    print(f"expected (approx): 2.0") # it is approx 2
