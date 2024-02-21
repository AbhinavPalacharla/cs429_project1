# import os

# TRACE_FILE = "./traces/cc.trace"

# TOTAL_CACHE_SIZE = 1024
# BLOCK_SIZE = 32

# assocs = [1, 2, 4, 8, 16, 32]
# # assocs = [1, 32]
# # assocs = [2, 4, 8, 16]

# for set_assoc in assocs:
#     os.system(f"./a.out {TRACE_FILE} {TOTAL_CACHE_SIZE} {BLOCK_SIZE} {set_assoc}")

# import os
import subprocess
import pandas as pd
import json
import matplotlib.pyplot as plt

# TRACE_FILES = ["./traces/cc.trace", "./traces/spice.trace", "./traces/tex.trace"]
TRACE_FILES = ["./traces/cc.trace"]

# L1_TOTAL_CACHE_SIZE = 1024
L1_TOTAL_CACHE_SIZE = 1024
L1_BLOCK_SIZE = 128
# L1_ASSOC = [1, 2, 4, 8, 16, 32]
L1_ASSOC = [1]

L2_TOTAL_CACHE_SIZE = 16384
# L2_BLOCK_SIZE = 128
L2_BLOCK_SIZES = [1, 2, 4, 8, 16, 32]
# L2_ASSOC = [1, 4, 16, 64, 128]
L2_ASSOC = [1]

output_json = []

for trace_file in TRACE_FILES:
    for l1_assoc in L1_ASSOC:
        for l2_assoc in L2_ASSOC:
            for l2_block_size in L2_BLOCK_SIZES:
                command = f"./multi.out {trace_file} {L1_TOTAL_CACHE_SIZE} {L1_BLOCK_SIZE} {l1_assoc} {L2_TOTAL_CACHE_SIZE} {l2_block_size} {l2_assoc}"
                # command = f"./multi.out {trace_file} {L1_TOTAL_CACHE_SIZE} {l1_block_size} {l1_assoc} {L2_TOTAL_CACHE_SIZE} {L2_BLOCK_SIZE} {l2_assoc}"

                result = subprocess.run(
                    command, shell=True, capture_output=True, text=True
                )

                if result.returncode == 0:
                    print(result.stdout)

                    json_data = json.loads(result.stdout)
                    json_data["L2_BLOCK_SIZE"] = l2_block_size

                    output_json.append(json_data)

# print(output_json[0])
data = []
for json_string in output_json:
    data.append(json_string)

# Create a DataFrame from the list of dictionaries
df = pd.DataFrame(data)

# df.to_csv("multi.csv", index=False)

plt.plot(df["L2_BLOCK_SIZE"], df["AMAT"], marker="o", linestyle="-")
# plt.bar(df["L1_BLOCK_SIZE"], df["AMAT"])
plt.xlabel("L2 BLOCK SIZE")
plt.ylabel("AMAT")
plt.title("Plot of AMAT vs L2 block size")
plt.grid(True)
plt.show()

df.to_csv("multi_l2_vary_block_sz", index=False)


# # import os

# # TRACE_FILE = "./traces/cc.trace"

# # TOTAL_CACHE_SIZE = 1024
# # BLOCK_SIZE = 32

# # assocs = [1, 2, 4, 8, 16, 32]
# # # assocs = [1, 32]
# # # assocs = [2, 4, 8, 16]

# # for set_assoc in assocs:
# #     os.system(f"./a.out {TRACE_FILE} {TOTAL_CACHE_SIZE} {BLOCK_SIZE} {set_assoc}")

# # import os
# import subprocess
# import pandas as pd
# import json
# import matplotlib.pyplot as plt

# # TRACE_FILES = ["./traces/cc.trace", "./traces/spice.trace", "./traces/tex.trace"]
# # TRACE_FILES = ["./traces/cc.trace"]

# # L1_TOTAL_CACHE_SIZE = 1024
# L1_TOTAL_CACHE_SIZE = 1024
# # L1_BLOCK_SIZE = 16
# L1_BLOCK_SIZES = [2, 4, 8, 16, 32, 64, 128, 256, 512]
# # L1_ASSOC = [1, 2, 4, 8, 16, 32]
# L1_ASSOC = [1]

# L2_TOTAL_CACHE_SIZE = 16384
# L2_BLOCK_SIZE = 128
# # L2_ASSOC = [1, 4, 16, 64, 128]
# L2_ASSOC = [128]

# output_json = []

# for trace_file in TRACE_FILES:
#     for l1_block_size in L1_BLOCK_SIZES:
#         for l1_assoc in L1_ASSOC:
#             for l2_assoc in L2_ASSOC:
#                 # command = f"./multi.out {trace_file} {L1_TOTAL_CACHE_SIZE} {L1_BLOCK_SIZE} {l1_assoc} {L2_TOTAL_CACHE_SIZE} {L2_BLOCK_SIZE} {l2_assoc}"
#                 command = f"./multi.out {trace_file} {L1_TOTAL_CACHE_SIZE} {l1_block_size} {l1_assoc} {L2_TOTAL_CACHE_SIZE} {L2_BLOCK_SIZE} {l2_assoc}"

#                 result = subprocess.run(
#                     command, shell=True, capture_output=True, text=True
#                 )

#                 if result.returncode == 0:
#                     print(result.stdout)

#                     json_data = json.loads(result.stdout)
#                     json_data["L1_BLOCK_SIZE"] = l1_block_size

#                     output_json.append(json_data)

# # print(output_json[0])
# data = []
# for json_string in output_json:
#     data.append(json_string)

# # Create a DataFrame from the list of dictionaries
# df = pd.DataFrame(data)

# # df.to_csv("multi.csv", index=False)

# plt.plot(df["L1_BLOCK_SIZE"], df["AMAT"], marker="o", linestyle="-")
# # plt.bar(df["L1_BLOCK_SIZE"], df["AMAT"])
# plt.xlabel("L1 block size")
# plt.ylabel("AMAT")
# plt.title("Plot of AMAT vs L1 block size")
# plt.grid(True)
# plt.show()

# df.to_csv("multi_l1_vary_block_sz.csv", index=False)
