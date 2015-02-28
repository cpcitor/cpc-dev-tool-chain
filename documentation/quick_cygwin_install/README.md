* [Next: install instructions -> After system preparation](../how_to_install.md#after-system-preparation)
* [Back to main documentation](../../README.md)

### How to quickly get all prerequisites on Cygwin

Download setup-x86.exe from cygwin.com and store in any location (Windows suggests your desktop folder).

(Optional: store it or move it after download to some folder of your choice, e.g. a new empty folder. The installer will keep a cache of downloaded files, by default next to the installer. This might help if for any reason you'd like to start over without downloading again.)

![Download setup-x86.exe from cygwin.com](img/08h50m42s_838688034.png)

Run installer.

![Run installer.](img/08h50m57s_742258801.png)

Installer will ask to select root install directory. Default is fine.

`cpc-dev-tool-chain` doesn't need administrator permissions, so if you're running from a non-privileged account, cygwin will automatically select the "Just Me" option.

![Select root install directory](img/08h51m01s_575552534.png)

Optional at step below: select any package you may need for your own use of Cygwin. Else just press "Next".

![Just press 'next'](img/08h52m00s_844526293.png)

Now a base cygwin system is installed.  We'll run a command line to install packages needed by `cpc-dev-tool-chain`.
In start menu choose "Run..."

![FIXME alt text img/09h17m55s_048931610.png](img/09h17m55s_048931610.png)

Type "cmd".

![FIXME alt text img/09h17m55s_451573309.png](img/09h17m55s_451573309.png)

Drag-n-drop the `setup-x86` icon onto the window.  This will automatically type the correct path.

![Drag-n-drop](img/09h18m45s_599936216.png)

After drag-n-drop you'll see the path:

![After drag-n-drop.](img/09h18m55s_191953264.png)

Now select the text below and use right click, key shortcut, whatever, to copy it to your clipboard.

<pre>
#Don't select the current line for copying, only the line below starting with -q...
     -q -P git,make,patch,gcc,gcc-core,gcc-g++,wget,bison,flex,libboost-devel,unrar,libncurses-devel,ioperm

</pre>

Right click in terminal then select "paste".

![FIXME alt text img/09h19m25s_916488745.png](img/09h19m25s_916488745.png)

The result will be a complete command line, like below.

![FIXME alt text img/09h19m41s_894913094.png](img/09h19m41s_894913094.png)

Press Enter.  The needed packages will automatically download and install.

![FIXME alt text img/09h23m23s_869659126.png](img/09h23m23s_869659126.png)

Congratulations, your Windows is ready to host `cpc-dev-tool-chain`.

### Next

* [Next: install instructions -> After system preparation](../how_to_install.md#after-system-preparation)
* [Back to main documentation](../../README.md)
