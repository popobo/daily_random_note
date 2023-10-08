# Wrap32::wrap( value, isn ): 240447159
# Wrap32::wrap( value, isn ).unwrap( isn, checkpoint ): 5361395264150972605
# Expected unwrap(wrap()) to recover same value, and it didn't!
#   unwrap(wrap(value, isn), isn, checkpoint) did not equal value
#   where value = 5361395268445939901, isn = Wrap32<3525756410>, and checkpoint = 5361395267314238352
#   (Difference between value and checkpoint is 1131701549.)

def decompose_number(num):
    two_pow_32 = 2 ** 32
    n = num // two_pow_32
    m = num % two_pow_32
    return n, m

# Wrap32::wrap( value, isn )
number = 240447159
n, m = decompose_number(number)
print(f"{number} = {n} * 2 ^ 32 + {m}")

# Wrap32::wrap( value, isn ).unwrap( isn, checkpoint )
number = 5361395264150972605
n, m = decompose_number(number)
print(f"{number} = {n} * 2 ^ 32 + {m}")

# value
number = 5361395268445939901
n, m = decompose_number(number)
print(f"{number} = {n} * 2 ^ 32 + {m}")

# checkpoint
number = 5361395267314238352
n, m = decompose_number(number)
print(f"{number} = {n} * 2 ^ 32 + {m}")

# difference
number = 1131701549
n, m = decompose_number(number)
print(f"{number} = {n} * 2 ^ 32 + {m}")



