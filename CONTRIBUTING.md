# Contributing

## How to Contribute

Please check your codes by the following commands.


1. format codes with `astyle`.

```
$ make astyle
```

2. create doxygen file.

```
$ make doxygen
```

3. please set following environment variables for your e-mail address.

```
$ export EMAIL_FROM_ADDRESS=YOUR_EMAIL_FROM_ADDRESS
$ export EMAIL_TO_ADDRESS=YOUR_EMAIL_TO_ADDRESS
```

4. do build tests for all configurations. If warning/errors occur, the results are in `testconfig/`.

```
$ make testconfig
```

If warning/errors occur, please revise your codes.
Otherwise, please send a pull request.
