import subprocess
import json
import argparse

# name that will be used if none specified
# this is the name used in docker-compose.yml
DEFAULT_NETWORK_NAME = 'cfs-demo_sat-link'

# get args
parser = argparse.ArgumentParser(description='Lookup IPs within a Docker network')
parser.add_argument('-name', '-n', default=DEFAULT_NETWORK_NAME, help='The name of the Docker network')
args = parser.parse_args()
network = args.name

# get the result of the docker network inspect command as utf-8 (text)
result = subprocess.run(['docker', 'network', 'inspect', '--format', "{{range .Containers}}{{.Name}}:{{.IPv4Address}},{{end}}", network], stdout=subprocess.PIPE).stdout.decode('utf-8')

# grab each node name (comma separated)
nodes = result.split(',')[:-1]

# each node is formatted as {hostname}:{ip}
for node in nodes:
    parsed = node.split(':')
    hostname = parsed[0]
    ip = parsed[1].split('/')[0] # remove the /16 at the end, no clue what it means
    print(hostname + ": " + ip)