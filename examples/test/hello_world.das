// options log

require daslib/jobque_boost

struct Work
    x, t : int

[export]
def test
    with_job_que() <|
        with_channel(5) <| $ ( channel )
            for x in range(5)
                new_job <| @
                    for t in range(3)
                        channel |> push_clone ( [[Work x=x, t=t]] )
                    channel |> notify                               // at this point we are done with the channel on this job
            var summ = 0
            for w in each(channel,type<Work>)
                summ += w.x * w.t
            assert(summ==30)
            assert(channel.isEmpty)
            assert(channel.isReady)
    return true
