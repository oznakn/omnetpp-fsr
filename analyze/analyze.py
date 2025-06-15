import os
import pathlib
import numpy as np

HOST_COUNT = 4
HOST_LETTERS = [chr(65 + i) for i in range(HOST_COUNT)]
APPS = [f"FSRNetwork.host{x}.app[0]" for x in HOST_LETTERS] + \
         [f"FSRNetwork.host{x}.fsr" for x in HOST_LETTERS]

TARGET = "FSRNetwork.hostD.app[0]"

MEASURES = {
    "packetReceived": {
        "unit": "packets",
        "measure": "p95_group",
        "f": lambda x: float(x),
    },
    "packetSent": {
        "unit": "packets",
        "measure": "p95_group",
        "f": lambda x: float(x),
    },
    "packetLengths": {
        "unit": "bits",
        "measure": "sum_avg",
        "f": lambda x: float(x),
    },
    "throughput":{
        "unit": "kbps",
        "measure": "mean", # p95
        "f": lambda x: float(x) / 1000,
    },
    "endToEndDelay": {
        "unit": "ms",
        "measure": "p95",
        "f": lambda x: float(x) * 1000,
    },
}
CALCULATIONS = {
    "packetDeliveryRatio": {
        "f": lambda x: (float(x[f"{TARGET}:packetReceived"][0]) / float(x["FSRNetwork.hostA.app[0]:packetSent"][0]) * 100, "%"),
    },
    "controlBits": {
        "f": lambda x: (sum(list(map(lambda y: float(x[y][0]) ,[f"FSRNetwork.host{x}.fsr:packetLengths" for x in HOST_LETTERS]))) / len(HOST_LETTERS), "bits"),
    }
}

BASE_PATH = pathlib.Path(__file__).parent.parent.resolve()

if __name__ == "__main__":
    data = {}
    vec_assignment = {}

    for i in range(50):
        filename = f"{BASE_PATH}/example/results/General-#{i}.vec"
        if not os.path.exists(filename):
            continue

        with open(filename, "r") as file:
            lines = file.readlines()

            for line in lines:
                if "vector" in line:
                    parts = line.split()

                    if parts[2] not in APPS:
                        continue

                    if parts[1] not in vec_assignment:
                        vec_assignment[parts[1]] = f"{parts[2]}:{parts[3]}"
                    if parts[1] not in data:
                        data[parts[1]] = []

                elif "0" <= line[0] <= "9":
                    parts = line.split()

                    if parts[0] in vec_assignment:
                        data[parts[0]].append({
                            "value": float(parts[3]),
                            "time": float(parts[2]),
                            "group": i,
                        })
        break

    results = {}

    for k, v in data.items():
        values = [item["value"] for item in v]
        if len(values) == 0:
            continue

        key = vec_assignment[k]

        npa = np.array(values)
        mean = np.mean(npa)
        p95 = np.percentile(npa, 95)
        sum_v = np.sum(npa)
        sum_avg = sum_v / len(npa) if len(npa) > 0 else 0
        count = len(v)

        values_by_group = {}
        for item in v:
            group = item["group"]
            if group not in values_by_group:
                values_by_group[group] = []
            values_by_group[group].append(item["value"])

        group_lengths = [len(values_by_group[group]) for group in values_by_group]
        npga = np.array(group_lengths)
        mean_group = np.mean(npga)
        p95_group = np.percentile(npga, 95)
        count_group = len(values_by_group)

        results[key] = {
            "mean": str(mean),
            "count": str(count),
            "p95": str(p95),
            "sum": str(sum_v),
            "sum_avg": str(sum_avg),
            "mean_group": str(mean_group),
            "p95_group": str(p95_group),
            "count_group": str(count_group),
        }

    outputs = {}
    for result_key, result in results.items():
        key_parts = result_key.split(":")
        if key_parts[1] not in MEASURES:
            continue

        measure = MEASURES[key_parts[1]]
        key = ":".join(key_parts[:2])

        value = measure["f"](result[measure["measure"]])
        unit = measure["unit"]

        outputs[key] = value, unit

    for calc_key, calc in CALCULATIONS.items():
        outputs[f"__CALC__{calc_key}"] = calc["f"](outputs)

    for key, (value, unit) in outputs.items():
        print(f"{key} = {value} {unit}")
