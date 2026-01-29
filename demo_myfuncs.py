


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

    
def make_time_list(t0, t1, dt):
    # inclusive t1 (with small tolerance)
    t_list = []
    t = t0
    # doing this to avoid float drift causing missing last point
    while t <= t1 + 1e-12:
        t_list.append(t)
        t = t + dt
    return t_list

def logistic_exact(t_list, r, K, P0, exp_terms=20):
    # this part returns list of P(t) for each t in t_list using exact closed form
    out = []
    alpha = (K - P0) / P0  # (K-P0)/P0
    for t in t_list:
        e_term = exponential(-r * t, exp_terms)
        denom = 1.0 + alpha * e_term
        out.append(K / denom)
    return out

def logistic_euler(t0, t1, dt, r, K, P0):
    # forward Euler is P_{n+1} = P_n + dt * r*P_n*(1 - P_n/K)
    t_list = make_time_list(t0, t1, dt)
    p_list = []

    P = P0
    for t in t_list:
        p_list.append(P)
        f = r * P * (1.0 - P / K)
        P = P + dt * f

    return t_list, p_list    




def time_hit_halfK_exact(r, K, P0, ln_iters=25, exp_terms=25):
    # Solve formula for P(t)=K/2:
    # K/2 = K / (1 + alpha e^{-rt}) -> 1 + alpha e^{-rt} = 2
    # alpha e^{-rt} = 1 -> e^{-rt} = 1/alpha -> t = (1/r) ln(alpha)
    alpha = (K - P0) / P0
    ln_alpha = nat_log(alpha, ln_iters, exp_terms)
    return ln_alpha / r


def time_hit_halfK_euler(t_list, p_list, K):
    # Find first time where P crosses K/2, do linear interpolation
    half = 0.5 * K
    for i in range(1, len(p_list)):
        p_prev = p_list[i - 1]
        p_curr = p_list[i]
        if (p_prev - half) == 0.0:
            return t_list[i - 1]
        if (p_prev < half and p_curr >= half) or (p_prev > half and p_curr <= half):
            t_prev = t_list[i - 1]
            t_curr = t_list[i]
            # linear interpolate between (t_prev, p_prev) and (t_curr, p_curr)
            if p_curr == p_prev:
                return t_curr
            frac = (half - p_prev) / (p_curr - p_prev)
            return t_prev + frac * (t_curr - t_prev)
    return None

RUN_OLD_TESTS = False

if __name__ == '__main__':

    if RUN_OLD_TESTS:
    # test factorial
        test_f = 5
        fact_res = factorial(test_f)
        print(f"--- factorial test ---")
        print(f"input: {test_f}")
        print(f"result: {fact_res}")
        print(f"expected: 120") #5! is 120, 6! = 720
        print("-" * 20)
    # test exp
        test_x = 2.0
        exp_res = exponential(test_x, 15)
        print(f"--- exp test ---")
        print(f"input: {test_x}")
        print(f"result: {exp_res}")
        print(f"expected (approx): 7.389056") # approx answer using calculator
        
        print("-" * 20)

    # test nat log
        test_ln_x = 7.389056
        # We use 20 iterations for Newton and 15 terms for the internal exp
        ln_res = nat_log(test_ln_x, 20, 15)
        print(f"--- nat log test ---")
        print(f"input: {test_ln_x}")
        print(f"result: {ln_res}")
        print(f"expected (approx): 2.0") # it is approx 2 since inverse of the one above


# lecture4 said r=0.5, K=100, P0=10, t in [0,20]
    r = 0.5
    K = 100.0
    P0 = 10.0
    t0 = 0.0
    t1 = 20.0
    dt = 0.1

    t_list = make_time_list(t0, t1, dt)
    p_exact = logistic_exact(t_list, r, K, P0, exp_terms=25)
    t_eul, p_eul = logistic_euler(t0, t1, dt, r, K, P0)

    t_half_exact = time_hit_halfK_exact(r, K, P0, ln_iters=30, exp_terms=30)
    t_half_euler = time_hit_halfK_euler(t_eul, p_eul, K)

    print("--- logistic growth ---")
    print("Example: P_exact(t=0) =", p_exact[0], "(expected P0 =", P0, ")")
    print("Example: P_euler(t=0) =", p_eul[0], "(expected P0 =", P0, ")")
    print()
    print("time when P hits K/2 (exact) =", t_half_exact)
    print("time when P hits K/2 (euler) =", t_half_euler)