for i in $(seq 1000); do echo $i; python3 ./test.py; python3 ./mso2004.py ; done | tee /tmp/test.log
