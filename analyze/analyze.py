import os
import pathlib
import numpy as np

def calculate_control_bits(data, configs):
    keys = data.keys()
    keys = list(filter(lambda x: ".fsr:packetLengths" in x, keys))

    values = list(map(lambda x: float(data[x][0]), keys))
    return sum(values) / len(values) if values else 0

def calculate_packet_delivery_ratio(data, configs):
    source = configs["source"]
    target = configs["target"]

    target_received = float(data[f"{target}:packetReceived"][0])
    source_sent = float(data[f"{source}:packetSent"][0])
    return (target_received / source_sent * 100) if source_sent > 0 else 0

NETWORKS = ["A", "B", "C", "D", "E", "F"]
APPS = ["fsr", "app[0]"]

SOURCE_TARGET_FINDER = ".app[0].typename"
SOURCE_NAME = "UdpBasicApp"
TARGET_NAME = "UdpSink"

# CONFIGS = ["**.wlan[0].bitrate"]
SETUPS = [
    {
        "folder": "topology-results",
        "configs": [],
    }
]

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
        "measure": "p95",
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
        "f": lambda data, configs: (calculate_packet_delivery_ratio(data, configs), "%"),
    },
    "controlBits": {
        "f": lambda data, configs: (calculate_control_bits(data, configs), "bits"),
    }
}

BASE_PATH = pathlib.Path(__file__).parent.parent.resolve()

if __name__ == "__main__":
    for setup_config in SETUPS:
        folder = setup_config["folder"]
        base_path = f"{BASE_PATH}/example/{folder}"
        filenames = os.listdir(base_path)

        for network in NETWORKS:
            network_name = f"FSRNetwork{network}"

            source = None
            target = None

            data = {}
            vec_assignment = {}

            for filename in filenames:
                if '.vec' not in filename or not filename.startswith(network_name):
                    continue

                filename = f"{base_path}/{filename}"
                run_id = filename.split("#")[1].split(".")[0]
                configs = {}
                group_data = {}

                with open(filename, "r") as file:
                    lines = file.readlines()

                    for line in lines:
                        if line.startswith("config"):
                            parts = line.split()
                            if SOURCE_TARGET_FINDER in parts[1]:
                                if parts[2] == SOURCE_NAME:
                                    source = parts[1].split(".")[1]
                                elif parts[2] == TARGET_NAME:
                                    target = parts[1].split(".")[1]

                            elif parts[1] in setup_config["configs"]:
                                configs[parts[1].split(".")[-1]] = parts[2]

                        elif line.startswith("vector"):
                            parts = line.split()

                            if parts[2].split(".")[-1] not in APPS:
                                continue

                            if parts[1] not in vec_assignment:
                                vec_assignment[parts[1]] = f"{parts[2]}:{parts[3]}"

                            if parts[1] not in group_data:
                                group_data[parts[1]] = []

                        elif "0" <= line[0] <= "9":
                            parts = line.split()

                            if parts[0] in vec_assignment:
                                group_data[parts[0]].append({
                                    "value": float(parts[3]),
                                    "time": float(parts[2]),
                                    "group": run_id,
                                })

                config_key = "__".join([f"{k}__{v}" for k, v in configs.items()])
                data[config_key] = group_data

            if len(data) == 0:
                continue

            for config_key, group_data in data.items():
                print("DATA FOR:", folder, network_name, config_key)

                results = {}
                for k, v in group_data.items():
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

                calculation_configs = {
                    "source": f"{network_name}.{source}.app[0]",
                    "target": f"{network_name}.{target}.app[0]",
                }
                for calc_key, calc in CALCULATIONS.items():
                    outputs[calc_key] = calc["f"](outputs, calculation_configs)

                for key, (value, unit) in outputs.items():
                    print(f"{key} = {value} {unit}")


                print("\n\n")
