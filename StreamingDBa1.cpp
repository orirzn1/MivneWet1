#include "StreamingDBa1.h"

streaming_database::streaming_database() : movieTree(), userTree(), groupTree(), comedyTree(), dramaTree(), actionTree(), fantasyTree(), noneTree(){
}

streaming_database::~streaming_database(){
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(genre == Genre::NONE || movieId <= 0 || views < 0)
        return StatusType::INVALID_INPUT;
    
    movieData* data = new movieData(movieId, genre, views, vipOnly);
    
    try
    {
        movieTree.insert(data, movieId);
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
            case Genre::NONE:
                noneTree.insert(data, *data);
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
        movieData data = *(movieTree.findNode(movieId)->data);
        movieTree.remove(movieId);
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
            case Genre::NONE:
                noneTree.remove(data);
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

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0)
        return StatusType::INVALID_INPUT;
    
    userData* data = new userData(userId, isVip);
    try
    {
        userTree.insert(data, userId);
        userTree.printTree();
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
        userData data = *(userTree.findNode(userId)->data);
        if(data.group)
        {
            (data.group->user_count)--;
            for(int i = 0; i < 5; i++)
            {
                data.group->total_views[i] -= (data.group->group_watches[i] - data.group_watches_before_joining[i]);
            }
            if(data.vipStatus)
                data.group->VIP_count--;
            data.group->users.remove(userId);
        }
        userTree.remove(userId);
        userTree.printTree();
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

    groupData* data = new groupData(groupId);
    
    try
    {
        groupTree.insert(data, groupId);
        groupTree.printTree();
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
        userData* user = (userTree.findNode(userId))->data;
        if(user->group != nullptr)
        {
            return StatusType::FAILURE;
        }
        groupData* group = (groupTree.findNode(groupId))->data;
        group->add_user(user);
        for(int i = 0; i < 5; i++)
        {
            user->group_watches_before_joining[i] = group->group_watches[i];
        }
        group->users.printTree(); 
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
        movieData* movie = (movieTree.findNode(movieId))->data;
        userData* user = (userTree.findNode(userId))->data;
        if(movie->vipOnly && !(user->vipStatus))
            return StatusType::FAILURE;
        (movie->views)++;
        switch(movie->genre)
        {
            case Genre::COMEDY:
                user->views[0]++;
                if(user->group)
                {
                    user->group->group_watches[0]++;
                    user->group->total_views[0] += user->group->user_count;
                }
                break;
            case Genre::DRAMA:
                user->views[1]++;
                if(user->group)
                {
                    user->group->group_watches[1]++;
                    user->group->total_views[1] += user->group->user_count;
                }
                break;
            case Genre::ACTION:
                user->views[2]++;
                if(user->group)
                {
                    user->group->group_watches[2]++;
                    user->group->total_views[2] += user->group->user_count;
                }
                break;
            case Genre::FANTASY:
                user->views[3]++;
                if(user->group)
                {
                    user->group->group_watches[3]++;
                    user->group->total_views[3] += user->group->user_count;
                }
                break;
            case Genre::NONE:
                user->views[4]++;
                if(user->group)
                {
                    user->group->group_watches[4]++;
                    user->group->total_views[4] += user->group->user_count;
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
    
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if(groupId <=0 || movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        movieData* movie = (movieTree.findNode(movieId))->data;
        groupData* group = (groupTree.findNode(groupId))->data;
        if((movie->vipOnly && (group->VIP_count == 0)) || (group->user_count == 0))
            return StatusType::FAILURE;
        movie->views += group->user_count;
        switch(movie->genre)
        {
            case Genre::COMEDY:
                group->group_watches[0]++;
                group->total_views[0] += group->user_count;
                break;
            case Genre::DRAMA:
                group->group_watches[1]++;
                group->total_views[1] += group->user_count;
                break;
            case Genre::ACTION:
                group->group_watches[2]++;
                group->total_views[2] += group->user_count;
                break;
            case Genre::FANTASY:
                group->group_watches[3]++;
                group->total_views[3] += group->user_count;
                break;
            case Genre::NONE:
                group->group_watches[4]++;
                group->total_views[4] += group->user_count;
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
            return output_t<int>(noneTree.getCount());
            break;
    }
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if(output == nullptr)
        return StatusType::INVALID_INPUT;
    
    switch(genre)
    {
        case Genre::COMEDY:
            return comedyTree.insertDescendingOrder(output);
            break;
        case Genre::DRAMA:
            return dramaTree.insertDescendingOrder(output);
            break;
        case Genre::ACTION:
            return actionTree.insertDescendingOrder(output);
            break;
        case Genre::FANTASY:
            return fantasyTree.insertDescendingOrder(output);
            break;
        default:
            return movieTree.insertDescendingOrder(output);
            break;
    }
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if(userId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try
    {
        userData* user = (userTree.findNode(userId))->data;
        switch(genre)
        {
            case Genre::COMEDY:
                return output_t<int>(user->views[0]);
                break;
            case Genre::DRAMA:
                return output_t<int>(user->views[1]);
                break;
            case Genre::ACTION:
                return output_t<int>(user->views[2]);
                break;
            case Genre::FANTASY:
                return output_t<int>(user->views[3]);
                break;
            default:
                return output_t<int>(user->views[4]);
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
    if(rating < 0 || rating > 100 || userId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        movieData* movie = movieTree.findNode(movieId)->data;
        userData* user = userTree.findNode(userId)->data;
        if(movie->vipOnly && !(user->vipStatus))
            return StatusType::FAILURE;
        switch(movie->genre)
        {
            case Genre::COMEDY:
                comedyTree.remove(*movie);
                movie->rating = (movie->rating + rating)/(++movie->num_of_ratings);
                comedyTree.insert(movie, *movie);
                break;
            case Genre::DRAMA:
                dramaTree.remove(*movie);
                movie->rating = (movie->rating + rating)/(++movie->num_of_ratings);
                dramaTree.insert(movie, *movie);
                break;
            case Genre::ACTION:
                actionTree.remove(*movie);
                movie->rating = (movie->rating + rating)/(++movie->num_of_ratings);
                actionTree.insert(movie, *movie);
                break;
            case Genre::FANTASY:
                fantasyTree.remove(*movie);
                movie->rating = (movie->rating + rating)/(++movie->num_of_ratings);
                fantasyTree.insert(movie, *movie);
                break;
            case Genre::NONE:
                noneTree.remove(*movie);
                movie->rating = (movie->rating + rating)/(++movie->num_of_ratings);
                noneTree.insert(movie, *movie);
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
        groupData* group = groupTree.findNode(groupId)->data;
        Genre genre = group->findFavoriteGenre();
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
                movieID = noneTree.getLargestNodeID();
                if(movieID != 0)
                    return output_t<int>(movieID);
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


