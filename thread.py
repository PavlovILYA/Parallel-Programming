import threading
import time
import math


def f(x):
    return math.sin(x) * x + math.cos(x * x) * x * x


mutex = threading.Lock()


def thread_function(id, begin, end):
    # print("начали")
    x = begin
    cur_min = f(x)
    x += 0.0001
    # time.sleep(1)

    while x < end:
        cur_f = f(x)
        if cur_f < cur_min:
            cur_min = cur_f
        x += 0.0001

    # std::lock_guard < std::mutex > guard(my_mutex);
    mutex.acquire()
    results[id] = cur_min
    mutex.release()
    # print("записали cur_min(f) = ", cur_min)


N = 4
results = [0, 0, 0, 0]

if __name__ == "__main__":
    begin = 0.0
    g_end = 100.0
    thrs = []

    for i in range(0, N):
        end = begin + g_end/N
        x = threading.Thread(target=thread_function, args=(i, begin, end,))
        thrs.append(x)
        x.start()
        # x.join()
        begin += g_end / N

    for t in thrs:
        t.join()

    cur_min = results[0]
    for i in results:
        if cur_min > i:
            cur_min = i

    print('python thread and mutex + global array')
    print('f = sin(x)*x+cos(x*x)*x*x')
    print('min(f) = ', cur_min)