#! /usr/bin/env python

import os
import sys

import gflags
import json

gflags.DEFINE_string('config', 'config.json', 'Your config (in JSON) for running application')
gflags.DEFINE_string('args', None, 'The arguments of job in \'k1=v1,k2=v2,...\' format')

gflags.FLAGS(sys.argv)

def start_worker(wid, worker_host, config):
    def get_existing_pid():
        check_existing_worker_cmd = 'ssh %s "cd %s && cat Worker.%s.pid 2> /dev/null"' % \
            (
                worker_host,
                config['app_scratch_dir'],
                wid
            )
        return os.popen(check_existing_worker_cmd).read().strip()

    # Create the scratch directory if it's not there
    init_app_scratch_dir_cmd = 'ssh %s "mkdir -p %s"' % \
        (
            worker_host,
            config['app_scratch_dir']
        )

    os.system(init_app_scratch_dir_cmd)

    # Chek whether there's already a running Worker
    existing_pid = get_existing_pid()
    if existing_pid != "":
        sys.stdout.write("Worker %s already started (PID: %s)\n" % (wid, existing_pid))
        return

    # Copy the worker list to Worker
    copy_list_to_worker_cmd = 'scp -q ./worker_list.txt %s:%s' % \
        (
            worker_host,
            config['app_scratch_dir']
        )

    os.system(copy_list_to_worker_cmd)

    # Start Worker
    start_worker_cmd = 'ssh %s "cd %s && bash --login -c \'%s --workers_info_path=%s --process_id=%s --app_scratch_dir=%s --log_dir=%s >%s 2>%s & echo \$! > Worker.%s.pid\'"&' % \
        (
            worker_host,
            config['app_scratch_dir'],
            os.path.join(config['app_bin_dir'], config['application']),
            './worker_list.txt',
            wid,
            config['app_scratch_dir'],
            config['app_scratch_dir'],
            os.path.join(config['app_scratch_dir'], 'stdout.{}'.format(wid)),
            os.path.join(config['app_scratch_dir'], 'stderr.{}'.format(wid)),
            wid
        )

    os.system(start_worker_cmd)

    sys.stdout.write("Worker %s started on %s.\n" % (wid, worker_host))

if __name__ == '__main__':
    config = json.load(open(gflags.FLAGS.config))
    with open('./worker_list.txt', 'w') as f:
        for w in config['workers']:
            f.write("%s:%s\n" % (w[0], w[1]))

    wid = 0
    for worker_host, _ in config['workers']:
        start_worker(wid, worker_host, config)
        wid += 1

    os.system('rm ./worker_list.txt')
