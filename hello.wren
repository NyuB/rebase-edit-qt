System.print("Hel-loaded")


class Hello {
    static do() {
        rollingMessage()
    }

    static setI(i) {
        __i = i
    }

    static rollingMessage() {
        __i = __i + 1

        if(__i >= 3) {
            __i = 0
        }

        if(__i == 0) {
            System.print("Yeepee")
        } else if (__i == 1) {
            System.print("Saperlipopette")
        } else if (__i == 2) {
            System.print("Oh wow ?!")
        }
    }
}

Hello.setI(0)
