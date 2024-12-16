[参考文章](https://smalloutcome.com/2021/07/18/Docker-%E4%BD%BF%E7%94%A8-macvlan-%E7%BD%91%E7%BB%9C%E5%AE%B9%E5%99%A8%E4%B8%8E%E5%AE%BF%E4%B8%BB%E6%9C%BA%E7%9A%84%E9%80%9A%E4%BF%A1%E8%BF%87%E7%A8%8B/)

```sh
ip link set eno1 promisc on

docker network create -d macvlan \
    --subnet=192.168.40.0/24 \
    --gateway=192.168.40.1 \
    --ip-range=192.168.40.16/28 \
    -o parent=eno1 vlan

ip link add link eno1 macvlan0-host type macvlan mode bridge
ip link set dev macvlan0-host up
ip route add 192.168.40.20 dev macvlan0-host

```