# Inputs for MF52 (B=3950)
vratio1 = 0.45
vratio2 = 0.80
ratio_tol1 = 0.01
ratio_tol2 = 0.05
Rntc_min = 4.2e3
Rntc_max = 26e3

# Stepping controls
step = 100  # Step size
startR = 100  # Step start
maxR1R2 = 250e3  # Step end
stop = 0

def get_ratio(Rntc, r1, r2):
    Rpara = (1.0 * Rntc * r2) / (Rntc + r2)
    Vratio = (1.0 * Rpara) / (Rpara + r1)
    return Vratio

def within_tol(num, val, tol):
    if num >= (val - tol) and num <= (val + tol):
        return True
    return False

def testTP4056Temp(Rntc1, Rntc2, Ratio1, Ratio2, tol1, tol2):
    global stop
    op = []
    found = 0
    for r1 in range(startR, int(maxR1R2), step):
        print(r1)
        for r2 in range(startR, int(maxR1R2), step):
            VratioCalc1 = get_ratio(Rntc1, r1, r2)

            if within_tol(VratioCalc1, Ratio1, tol1):
                VratioCalc2 = get_ratio(Rntc2, r1, r2)

                if within_tol(VratioCalc2, Ratio2, tol2):
                    frat1 = "{:.3f}".format(VratioCalc1)
                    frat2 = "{:.3f}".format(VratioCalc2)
                    print("Found", r1, r2, "Ratio1", frat1, "Ratio2", frat2)
                    op.append("Found {} {} Ratio1 {} Ratio2 {}\n".format(r1, r2, frat1, frat2))
                    found += 1
            if stop:
                break
        if stop:
            break
    with open("op.txt", "w") as fh:
        fh.write(''.join(op))

    return found

def stop():
    global stop
    stop = 1

stop_button = input("Press Enter to stop.")

found_count = testTP4056Temp(Rntc_min, Rntc_max, vratio1, vratio2, ratio_tol1, ratio_tol2)
print("\nFound:", found_count, "\n")
