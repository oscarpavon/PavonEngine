#!/usr/bin/expect -f
spawn git push
expect "Username"
send "oscarpavon\n"
expect "Password"
send "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\n"
interact


