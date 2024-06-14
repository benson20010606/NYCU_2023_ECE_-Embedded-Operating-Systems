#!/bin/bash

SESSION="lab6"
PORT=8888

make clean
make

tmux has-session -t $SESSION 2>/dev/null

if [[ $? -eq 0 ]]
then
	tmux kill-session -t $SESSION
fi

tmux new-session -d -s $SESSION

tmux split-window -h -p 80
tmux split-window -v -p 75
tmux split-window -v -p 66
tmux split-window -v -p 50

tmux send-keys -t 0 "./server $PORT" C-m
sleep 1

tmux send-keys -t 1 "./client 127.0.0.1 $PORT deposit 1 500" C-m
tmux send-keys -t 2 "./client 127.0.0.1 $PORT withdraw 1 500" C-m
tmux send-keys -t 3 "./client 127.0.0.1 $PORT deposit 2 500" C-m
tmux send-keys -t 4 "./client 127.0.0.1 $PORT withdraw 2 500" C-m

tmux -2 attach-session -t $SESSION
