#! /usr/bin/env python

import gflags
import json
import os
import sys

gflags.DEFINE_string('config', 'config.json', 'Your config (in JSON) for running Job')

gflags.FLAGS(sys.argv)

def stop_worker(wid, worker_host, config):
    def get_existing_pid():
        check_existing_worker_cmd = 'ssh %s "cat %s 2> /dev/null"' % \
            (
                worker_host,
                os.path.join(config['app_scratch_dir'], "Worker."+str(wid)+".pid")
            )
        return os.popen(check_existing_worker_cmd).read().strip()

    # Chek whether there's already a running Master
    existing_pid = get_existing_pid()
    if existing_pid == "":
        sys.stdout.write("Worker %s is not running\n" % wid)
        return

    # Stop Master and clear up
    stop_worker_cmd = 'ssh %s "kill %s"' % (worker_host, existing_pid)
    os.system(stop_worker_cmd)

    sys.stdout.write("Worker %s stopped.\n" % wid)

def clean_up_worker(wid, worker_host, config):
    rm_scratch_dir_cmd = 'ssh %s "rm -rf %s"' % (worker_host, config['app_scratch_dir'])
    os.system(rm_scratch_dir_cmd)

if __name__ == '__main__':
    config = json.load(open(gflags.FLAGS.config))

    wid = 0
    for worker_host, _ in config['workers']:
        stop_worker(wid, worker_host, config)
        wid += 1

    wid = 0
    for worker_host, _ in config['workers']:
        clean_up_worker(wid, worker_host, config)
        wid += 1
