from datetime import datetime
import time
import pyvisa as visa

class Scope():

    def __init__(self):

        resources = visa.ResourceManager('@py')

        self.scope = resources.open_resource('USB0::1689::924::C010251::0::INSTR', read_termination="\n", write_termination="\n")
        self.scope.timeout = 2000
        print(self.scope.query('*IDN?'))
        self.scope.write("SAVe:IMAGe:FILEFormat PNG")
        self.scope.write("SAVe:IMAGe:INKSaver OFF")

    def grab_png(self, image_name=None):
        if image_name is None:
            # Generate a filename based on the current Date & Time
            dt = datetime.now()
            fileName = dt.strftime("%Y%m%d_%H%M%S.png")

        print("grabbing image")
        print(self.scope.write("HARDCopy STARt"))
        imgData = self.scope.read_raw()

        with open(fileName, "wb") as fh:
            print("written to %s" % fileName)
            fh.write(imgData)

    def single(self):
        start_time = time.time()
        self.scope.write("TRIGger:MODe NORMal")         # set trigger normal
        self.scope.write("ACQuire:STOPAfter SEQ")       # single shot
        self.scope.write("ACQuire:STATE RUN")           # run
        time.sleep(1.0) # wait for arm
        print("waiting for trigger...")
        while True:
            state = self.scope.query("TRIGger:STATE?")  # get trigger state
            #print(state)
            if(state == "SAV"):
                break
            time.sleep(0.1)
        print("waited %d seconds" % (time.time() - start_time))

    def measure(self):
        self.scope.write('MEASUREMENT:IMMED:TYPE MAX')
        self.scope.write('MEASUREMENT:IMMED:SOURCE CH2')
        return float(self.scope.query('MEASUREMENT:IMMED:VALUE?'))

    def __del__(self):
        self.scope.close()


if __name__ == '__main__':
    scope = Scope()
    scope.single()
    print(scope.measure())
