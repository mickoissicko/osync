#include <iostream>
#include <thread>
#include <chrono>

void Cloque(int Local)
{
    constexpr int INTERVAL = 5;
    int PushCounter = 1;

    while (1) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(INTERVAL));

        if (std::system("git add .") != 0)
            std::cout << "Add fail\n";

        if (std::system("git commit -m 'Update'") != 0)
            std::cout << "Commit fail\n";

        if (PushCounter == 6)
        {
            if (Local)
                if (std::system("git push origin master") != 0)
                    std::cerr << "Push fail\n";

            else if (!Local)
                if (std::system("git push") != 0)
                    std::cerr << "Push fail (reg)\n";
        }

        PushCounter++;
    }
}
