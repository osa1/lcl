test_set = containers.newset()
print(test_set)
print(containers.set_insert(test_set, 10))
print(containers.set_insert(test_set, 10))
print(containers.set_insert(test_set, 10))
print(containers.set_insert(test_set, 20))
print(containers.set_count(test_set, 10))
print(containers.set_size(test_set))

test_queue = containers.queue_new()
print(test_queue)
--print(containers.queue_size())
containers.queue_push(test_queue, 1)
print(containers.queue_pop(test_queue))
