#include "StreamingDBa1.h"

streaming_database::streaming_database() : movieTree(), movieByRating(), userTree(), groupTree(), comedyTree(), dramaTree(), actionTree(), fantasyTree(){
}

streaming_database::~streaming_database(){
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(genre == Genre::NONE || movieId <= 0 || views < 0)
        return StatusType::INVALID_INPUT;
    
    std::shared_ptr<movieData> data = std::make_shared<movieData>(movieId, genre, views, vipOnly);
    
    try
    {
        movieTree.insert(data, movieId);
        movieByRating.insert(data, *data);
        switch(genre)
        {
            case Genre::COMEDY:
                comedyTree.insert(data, *data);
                break;
            case Genre::DRAMA:
                dramaTree.insert(data, *data);
                break;
            case Genre::ACTION:
                actionTree.insert(data, *data);
                break;
            case Genre::FANTASY:
                fantasyTree.insert(data, *data);
                break;
            default:
                break;

        }
            
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR; 
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if(movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        movieData data = *(movieTree.findNode(movieId)->data.get());
        Genre genre = data.genre;
        switch(genre)
        {
            case Genre::COMEDY:
                comedyTree.remove(data);
                break;
            case Genre::DRAMA:
                dramaTree.remove(data);
                break;
            case Genre::ACTION:
                actionTree.remove(data);
                break;
            case Genre::FANTASY:
                fantasyTree.remove(data);
                break;
            default:
                break;
        }
        movieByRating.remove(data);
        movieTree.remove(movieId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0)
        return StatusType::INVALID_INPUT;
    
    std::shared_ptr<userData> data = std::make_shared<userData>(userId, isVip);
    try
    {
        userTree.insert(data, userId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        userData data = *(userTree.findNode(userId)->data.get());

        if(data.group)
        {
            (data.group->user_count)--;
            for(int i = 0; i < 5; i++)
            {

                data.group->total_views[i] -= ((data.views[i] ) + (data.group->group_watches[i] - data.group_watches_before_joining[i]));
            }
            if(data.vipStatus)
                data.group->VIP_count--;
            data.group->users.remove(userId);
        }
        data.group = nullptr; 
        userTree.remove(userId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if(groupId <= 0)
        return StatusType::INVALID_INPUT;

    std::shared_ptr<groupData> data = std::make_shared<groupData>(groupId);
    
    try
    {
        groupTree.insert(data, groupId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if(groupId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        groupData* group = (groupTree.findNode(groupId)->data.get());
        groupTree.removeAllUsers(group->users.getRoot());
        groupTree.remove(groupId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if(userId <=0 || groupId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        userData* user = (userTree.findNode(userId)->data.get());
        if(user->group != nullptr)
        {
            return StatusType::FAILURE;
        }
        groupData* group = (groupTree.findNode(groupId)->data.get());
        group->add_user((userTree.findNode(userId)->data));
        for(int i = 0; i < 5; i++)
        {
            user->group_watches_before_joining[i] = group->group_watches[i];
            group->total_views[i] += user->views[i];
        }
        user->group = group;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if(userId <=0 || movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        std::shared_ptr<movieData> movie = (movieTree.findNode(movieId)->data);
        std::shared_ptr<userData> user = (userTree.findNode(userId)->data);
        if(movie->vipOnly && !(user->vipStatus))
            return StatusType::FAILURE;

        switch(movie->genre)
        {
            case Genre::COMEDY:
                user->views[0]++;
                user->views[4]++;
                if(user->group)
                {
                    user->group->total_views[0]++;
                }
                comedyTree.remove(*movie);
                movieByRating.remove(*movie);
                (movie->views)++;
                comedyTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);

                break;
            case Genre::DRAMA:
                user->views[1]++;
                user->views[4]++;
                if(user->group)
                {
                    user->group->total_views[1]++;
                }
                dramaTree.remove(*movie);
                movieByRating.remove(*movie);
                (movie->views)++;
                dramaTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::ACTION:
                user->views[2]++;
                user->views[4]++;
                if(user->group)
                {
                    user->group->total_views[2]++;
                }
                actionTree.remove(*movie);
                movieByRating.remove(*movie);
                (movie->views)++;
                actionTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::FANTASY:
                user->views[3]++;
                user->views[4]++;
                if(user->group)
                {
                    user->group->total_views[3]++;
                }
                fantasyTree.remove(*movie);
                movieByRating.remove(*movie);
                (movie->views)++;
                fantasyTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            default:
                break;
        }
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if(groupId <=0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    try
    {

        std::shared_ptr<movieData> movie = (movieTree.findNode(movieId)->data);
        std::shared_ptr<groupData> group = (groupTree.findNode(groupId)->data);
        if((movie->vipOnly && (group->VIP_count == 0)) || (group->user_count == 0)) {
            return StatusType::FAILURE;
        }
        switch(movie->genre)
        {
            case Genre::COMEDY:

                group->group_watches[0]++;
                group->total_views[0] += group->user_count;
                group->group_watches[4]++;
                group->total_views[4] += group->user_count;
                comedyTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->views += group->user_count;
                comedyTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::DRAMA:
                group->group_watches[1]++;
                group->total_views[1] += group->user_count;
                group->group_watches[4]++;
                group->total_views[4] += group->user_count;
                dramaTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->views += group->user_count;
                dramaTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::ACTION:
                group->group_watches[2]++;
                group->total_views[2] += group->user_count;
                group->group_watches[4]++;
                group->total_views[4] += group->user_count;
                actionTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->views += group->user_count;
                actionTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::FANTASY:
                group->group_watches[3]++;
                group->total_views[3] += group->user_count;
                group->group_watches[4]++;
                group->total_views[4] += group->user_count;
                fantasyTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->views += group->user_count;
                fantasyTree.insert(movie,*movie);
                movieByRating.insert(movie,*movie);
                break;
            default:
                break;
        }
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    switch(genre)
    {
        case Genre::COMEDY:
            return output_t<int>(comedyTree.getCount());
            break;
        case Genre::DRAMA:
            return output_t<int>(dramaTree.getCount());
            break;
        case Genre::ACTION:
            return output_t<int>(actionTree.getCount());
            break;
        case Genre::FANTASY:
            return output_t<int>(fantasyTree.getCount());
            break;
        default:
            return output_t<int>(comedyTree.getCount()+dramaTree.getCount()+actionTree.getCount()+fantasyTree.getCount());
            break;
    }
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if(output == nullptr)
        return StatusType::INVALID_INPUT;
    int* sortedMoviesArray;
    switch(genre)
    {
        case Genre::COMEDY:
            sortedMoviesArray = new int[comedyTree.getCount()];
            if(comedyTree.insertDescendingOrder(sortedMoviesArray) == StatusType::FAILURE)
            {
                delete[] sortedMoviesArray;
                return StatusType::FAILURE;
            }
            for (int i = 0; i < comedyTree.getCount(); i++) {
                output[i] = sortedMoviesArray[i];
            }
            delete[] sortedMoviesArray;
            return StatusType::SUCCESS;
            break;
        case Genre::DRAMA:
            sortedMoviesArray = new int[dramaTree.getCount()];
            if(dramaTree.insertDescendingOrder(sortedMoviesArray) == StatusType::FAILURE)
            {
                delete[] sortedMoviesArray;
                return StatusType::FAILURE;
            }
            for (int i = 0; i < dramaTree.getCount(); i++) {
                output[i] = sortedMoviesArray[i];
            }
            delete[] sortedMoviesArray;
            return StatusType::SUCCESS;
            break;
        case Genre::ACTION:
            sortedMoviesArray = new int[actionTree.getCount()];
            if(actionTree.insertDescendingOrder(sortedMoviesArray) == StatusType::FAILURE)
            {
                delete[] sortedMoviesArray;
                return StatusType::FAILURE;
            }
            for (int i = 0; i < actionTree.getCount(); i++) {
                output[i] = sortedMoviesArray[i];
            }
            delete[] sortedMoviesArray;
            return StatusType::SUCCESS;
            break;
        case Genre::FANTASY:
            sortedMoviesArray = new int[fantasyTree.getCount()];
            if(fantasyTree.insertDescendingOrder(sortedMoviesArray) == StatusType::FAILURE)
            {
                delete[] sortedMoviesArray;
                return StatusType::FAILURE;
            }
            for (int i = 0; i < fantasyTree.getCount(); i++) {
                output[i] = sortedMoviesArray[i];
            }
            delete[] sortedMoviesArray;
            return StatusType::SUCCESS;
            break;
        default:
            sortedMoviesArray = new int[movieByRating.getCount()];
            if(movieByRating.insertDescendingOrder(sortedMoviesArray) == StatusType::FAILURE)
            {
                delete[] sortedMoviesArray;
                return StatusType::FAILURE;
            }
            for (int i = 0; i < movieByRating.getCount(); i++) {
                output[i] = sortedMoviesArray[i];
            }
            delete[] sortedMoviesArray;
            return StatusType::SUCCESS;
            break;
    }
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if(userId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try
    {
        userData* user = (userTree.findNode(userId)->data.get());
        switch(genre)
        {
            case Genre::COMEDY:
                if (user->group != nullptr){
                    return output_t<int>(user->views[0] + (user->group->group_watches[0] - user->group_watches_before_joining[0]));
                } else{
                    return output_t<int>(user->views[0]);
                }
                break;
            case Genre::DRAMA:
                if (user->group != nullptr){
                    return output_t<int>(user->views[1] + (user->group->group_watches[1] - user->group_watches_before_joining[1]));
                } else{
                    return output_t<int>(user->views[1]);
                }
                break;
            case Genre::ACTION:
                if (user->group != nullptr){
                    return output_t<int>(user->views[2] + (user->group->group_watches[2] - user->group_watches_before_joining[2]));
                } else{
                    return output_t<int>(user->views[2]);
                }
                break;
            case Genre::FANTASY:
                if (user->group != nullptr){

                    return output_t<int>(user->views[3] + (user->group->group_watches[3] - user->group_watches_before_joining[3]));
                } else{
                    return output_t<int>(user->views[3]);
                }
                break;
            default:
                if (user->group != nullptr){
                    return output_t<int>(user->views[4] + (user->group->group_watches[4] - user->group_watches_before_joining[4]));
                } else{
                    return output_t<int>(user->views[4]);
                }
                break;
        }
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(rating < 0 || rating > 100 || userId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try
    {
        std::shared_ptr<movieData> movie = (movieTree.findNode(movieId)->data);
        userData* user = (userTree.findNode(userId)->data.get());
        if(movie->vipOnly && !(user->vipStatus))
            return StatusType::FAILURE;
        switch(movie->genre)
        {
            case Genre::COMEDY:
                comedyTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->rating = ((movie->rating)*movie->num_of_ratings + rating)/(++movie->num_of_ratings);
                comedyTree.insert(movie, *movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::DRAMA:
                dramaTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->rating = ((movie->rating)*movie->num_of_ratings + rating)/(++movie->num_of_ratings);
                dramaTree.insert(movie, *movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::ACTION:
                actionTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->rating = ((movie->rating)*movie->num_of_ratings + rating)/(++movie->num_of_ratings);
                actionTree.insert(movie, *movie);
                movieByRating.insert(movie,*movie);
                break;
            case Genre::FANTASY:
                fantasyTree.remove(*movie);
                movieByRating.remove(*movie);
                movie->rating = ((movie->rating)*movie->num_of_ratings + rating)/(++movie->num_of_ratings);
                fantasyTree.insert(movie, *movie);
                movieByRating.insert(movie,*movie);
                break;
            default:
                break;
        }
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if(groupId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);

    try
    {
        groupData* group = (groupTree.findNode(groupId)->data.get());
        if (group->user_count == 0){
            throw Failure();
        }
        Genre genre = group->findFavoriteGenre();
        if (get_all_movies_count(genre).ans() == 0){
            throw Failure();
        }
        int movieID;
        switch(genre)
        {
            case Genre::COMEDY:
                movieID = comedyTree.getLargestNodeID();
                if(movieID != 0)
                    return output_t<int>(movieID);
                return output_t<int>(StatusType::FAILURE);
                break;
            case Genre::DRAMA:
                movieID = dramaTree.getLargestNodeID();
                if(movieID != 0)
                    return output_t<int>(movieID);
                return output_t<int>(StatusType::FAILURE);
                break;
            case Genre::ACTION:
                movieID = actionTree.getLargestNodeID();
                if(movieID != 0)
                    return output_t<int>(movieID);
                return output_t<int>(StatusType::FAILURE);
                break;
            case Genre::FANTASY:
                movieID = fantasyTree.getLargestNodeID();
                if(movieID != 0)
                    return output_t<int>(movieID);
                return output_t<int>(StatusType::FAILURE);
                break;
            default:
                return output_t<int>(StatusType::FAILURE);
                break;
        }
    }
    catch(std::bad_alloc& e)
    {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    catch(Failure& e)
    {
        return output_t<int>(StatusType::FAILURE);
    }
    
}


