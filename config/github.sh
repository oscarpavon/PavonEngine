#!/usr/bin/expect -f
set password [lindex $argv 0]
spawn git push
expect "Username"
send "oscarpavon\n"
expect "Password"
send "$password\n"
interact


